const state = {
  accessToken: localStorage.getItem("tictac.accessToken") || "",
  refreshToken: localStorage.getItem("tictac.refreshToken") || "",
  user: null,
  game: null,
  gamePollingId: null,
  openGamesPollingId: null,
  gameSyncInProgress: false,
};

const elements = {
  notice: document.querySelector("#notice"),
  authSection: document.querySelector("#auth-section"),
  appSection: document.querySelector("#app-section"),
  signOut: document.querySelector("#sign-out-button"),
  userLogin: document.querySelector("#user-login"),
  userId: document.querySelector("#user-id"),
  leaderboardBody: document.querySelector("#leaderboard-body"),
  leaderboardLimit: document.querySelector("#leaderboard-limit"),
  board: document.querySelector("#board"),
  gameTitle: document.querySelector("#game-title"),
  gameStatus: document.querySelector("#game-status"),
  lobby: document.querySelector("#lobby"),
  lobbyTitle: document.querySelector("#lobby-title"),
  lobbyDetails: document.querySelector("#lobby-details"),
  lobbyId: document.querySelector("#lobby-id"),
  copyGameId: document.querySelector("#copy-game-id"),
  gameIdInput: document.querySelector("#game-id-input"),
  openGames: document.querySelector("#open-games-list"),
};

function showNotice(message = "", isError = false) {
  elements.notice.textContent = message;
  elements.notice.classList.toggle("error", isError);
}

function saveTokens(tokens) {
  state.accessToken = tokens.accessToken;
  state.refreshToken = tokens.refreshToken;
  localStorage.setItem("tictac.accessToken", state.accessToken);
  localStorage.setItem("tictac.refreshToken", state.refreshToken);
}

function clearSession() {
  stopGamePolling();
  stopOpenGamesPolling();
  state.accessToken = "";
  state.refreshToken = "";
  state.user = null;
  state.game = null;
  localStorage.removeItem("tictac.accessToken");
  localStorage.removeItem("tictac.refreshToken");
  elements.authSection.classList.remove("hidden");
  elements.appSection.classList.add("hidden");
  elements.signOut.classList.add("hidden");
  renderBoard();
}

async function parseResponse(response) {
  const body = await response.text();
  let data = null;
  try { data = body ? JSON.parse(body) : null; } catch { data = body; }
  if (!response.ok) {
    const message = typeof data === "string" && data ? data : `Ошибка запроса (${response.status})`;
    const error = new Error(message);
    error.status = response.status;
    throw error;
  }
  return data;
}

async function refreshAccessToken() {
  if (!state.refreshToken) return false;
  try {
    const response = await fetch("/refresh/access", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ refreshToken: state.refreshToken }),
    });
    saveTokens(await parseResponse(response));
    return true;
  } catch {
    return false;
  }
}

async function api(path, options = {}, retry = true) {
  const headers = new Headers(options.headers || {});
  if (state.accessToken) headers.set("Authorization", `Bearer ${state.accessToken}`);
  if (options.body && !headers.has("Content-Type")) headers.set("Content-Type", "application/json");
  try {
    const response = await fetch(path, { ...options, headers });
    return await parseResponse(response);
  } catch (error) {
    if (error.status === 401 && retry && await refreshAccessToken()) return api(path, options, false);
    if (error.status === 401) clearSession();
    throw error;
  }
}

function symbolForCurrentUser(game) {
  return game.players.find((player) => player.id === state.user?.id)?.symbol || 0;
}

function describeGame(game) {
  if (!game) return "Создайте игру с компьютером или подключитесь к открытой партии.";
  if (game.status.status === "waiting") return "Игра ждёт второго игрока. Передайте UUID сопернику.";
  if (game.status.status === "draw") return "Ничья.";
  if (game.status.status === "win") return game.status.idPlayer === state.user?.id ? "Вы победили!" : "Игра завершена: победил другой игрок.";
  if (game.status.idPlayer === state.user?.id) return `Ваш ход (${symbolForCurrentUser(game) === 1 ? "X" : "O"}).`;
  return "Ход соперника — состояние обновится автоматически.";
}

function renderLobby() {
  const game = state.game;
  const isPlayerGame = game?.gametype === "player";
  elements.lobby.classList.toggle("hidden", !isPlayerGame);
  if (!isPlayerGame) return;

  const playerCount = game.players.length;
  const yourSymbol = symbolForCurrentUser(game) === 1 ? "крестики (×)" : "нолики (○)";
  elements.lobbyId.textContent = game.id;

  if (game.status.status === "waiting") {
    elements.lobbyTitle.textContent = `Лобби: ${playerCount} из 2 игроков`;
    elements.lobbyDetails.textContent = `Вы играете за ${yourSymbol}. Ждём, пока соперник подключится.`;
    return;
  }

  if (game.status.status === "turn") {
    elements.lobbyTitle.textContent = "Лобби заполнено: 2 из 2 игроков";
    elements.lobbyDetails.textContent = `Вы играете за ${yourSymbol}. ${describeGame(game)}`;
    return;
  }

  elements.lobbyTitle.textContent = "Игра завершена";
  elements.lobbyDetails.textContent = describeGame(game);
}

function renderBoard() {
  elements.board.replaceChildren();
  const board = state.game?.board || Array.from({ length: 3 }, () => Array(3).fill(0));
  const canMove = state.game && state.game.status.status === "turn" && state.game.status.idPlayer === state.user?.id;
  board.forEach((row, rowIndex) => row.forEach((value, columnIndex) => {
    const button = document.createElement("button");
    button.type = "button";
    button.className = `cell ${value === 1 ? "x" : value === 2 ? "o" : ""}`;
    button.textContent = value === 1 ? "×" : value === 2 ? "○" : "";
    button.disabled = !canMove || value !== 0;
    button.setAttribute("aria-label", `Клетка ${rowIndex + 1}, ${columnIndex + 1}`);
    button.addEventListener("click", () => makeMove(rowIndex, columnIndex));
    elements.board.append(button);
  }));
  elements.gameTitle.textContent = state.game ? `Игра ${state.game.id.slice(0, 8)}` : "Новая партия";
  elements.gameStatus.textContent = describeGame(state.game);
  renderLobby();
}

function renderLeaderboard(players) {
  elements.leaderboardBody.replaceChildren();
  if (!players.length) {
    elements.leaderboardBody.innerHTML = '<tr><td colspan="6" class="muted">Пока нет завершённых игр.</td></tr>';
    return;
  }
  players.forEach((player, index) => {
    const row = document.createElement("tr");
    const values = [index + 1, player.login, player.wins, player.losses, player.draws, Number(player.winRatio).toFixed(2)];
    values.forEach((value) => {
      const cell = document.createElement("td");
      cell.textContent = String(value);
      row.append(cell);
    });
    elements.leaderboardBody.append(row);
  });
}

function renderOpenGames(games) {
  elements.openGames.replaceChildren();
  if (!games.length) {
    elements.openGames.innerHTML = '<li class="muted">Открытых игр пока нет.</li>';
    return;
  }
  games.forEach((id) => {
    const item = document.createElement("li");
    item.className = "game-row";
    const code = document.createElement("code");
    code.className = "game-id";
    code.textContent = id;
    const join = document.createElement("button");
    join.className = "button secondary";
    join.type = "button";
    const isOwnLobby = state.game?.id === id && state.game.status.status === "waiting";
    join.textContent = isOwnLobby ? "Открыть лобби" : "Подключиться";
    join.addEventListener("click", () => {
      const action = isOwnLobby ? showGame(id) : joinGame(id);
      action.catch(reportError);
    });
    item.append(code, join);
    elements.openGames.append(item);
  });
}

async function loadProfile() {
  state.user = await api("/user");
  elements.userLogin.textContent = state.user.login;
  elements.userId.textContent = state.user.id;
}

async function loadLeaderboard() {
  const limit = elements.leaderboardLimit.value;
  renderLeaderboard([]);
  const players = await api(`/user/top?limit=${encodeURIComponent(limit)}`);
  renderLeaderboard(players);
}

async function loadOpenGames({ quiet = false } = {}) {
  if (!quiet) elements.openGames.innerHTML = '<li class="muted">Загрузка…</li>';
  const games = await api("/game");
  renderOpenGames(Array.isArray(games) ? games : []);
}

function stopGamePolling() {
  if (state.gamePollingId !== null) window.clearInterval(state.gamePollingId);
  state.gamePollingId = null;
  state.gameSyncInProgress = false;
}

function updateGamePolling() {
  const shouldPoll = state.game?.gametype === "player" && ["waiting", "turn"].includes(state.game.status.status);
  if (!shouldPoll) {
    stopGamePolling();
    return;
  }
  if (state.gamePollingId === null) {
    state.gamePollingId = window.setInterval(() => refreshCurrentGame(), 1000);
  }
}

function stopOpenGamesPolling() {
  if (state.openGamesPollingId !== null) window.clearInterval(state.openGamesPollingId);
  state.openGamesPollingId = null;
}

function startOpenGamesPolling() {
  stopOpenGamesPolling();
  state.openGamesPollingId = window.setInterval(() => {
    if (!document.hidden) loadOpenGames({ quiet: true }).catch(() => {});
  }, 3000);
}

function setCurrentGame(game) {
  state.game = game;
  elements.gameIdInput.value = game.id;
  renderBoard();
  updateGamePolling();
}

async function refreshCurrentGame() {
  const game = state.game;
  if (!game || game.gametype !== "player" || state.gameSyncInProgress) return;

  state.gameSyncInProgress = true;
  try {
    const updatedGame = await api(`/game/${encodeURIComponent(game.id)}`);
    const opponentJoined = game.status.status === "waiting" && updatedGame.status.status === "turn";
    setCurrentGame(updatedGame);
    if (opponentJoined) {
      showNotice("Соперник подключился. Игра начинается!");
      loadOpenGames({ quiet: true }).catch(() => {});
    }
  } catch {
    // Фоновое обновление не должно мешать текущей игре сообщениями об ошибках.
  } finally {
    state.gameSyncInProgress = false;
  }
}

async function showGame(id) {
  setCurrentGame(await api(`/game/${encodeURIComponent(id)}`));
}

async function createGame(gametype) {
  setCurrentGame(await api("/game", { method: "POST", body: JSON.stringify({ gametype }) }));
  if (gametype === "player") await loadOpenGames();
  showNotice(gametype === "computer" ? "Игра с компьютером создана." : "Открытая игра создана. Передайте UUID сопернику.");
}

async function joinGame(id) {
  await api(`/game/${encodeURIComponent(id)}/join`, { method: "POST" });
  await showGame(id);
  await loadOpenGames();
  showNotice("Вы подключились к игре.");
}

async function makeMove(row, column) {
  const game = state.game;
  if (!game) return;
  const nextBoard = game.board.map((currentRow) => [...currentRow]);
  nextBoard[row][column] = symbolForCurrentUser(game);
  setCurrentGame(await api(`/game/${encodeURIComponent(game.id)}`, {
    method: "POST",
    body: JSON.stringify({ ...game, board: nextBoard }),
  }));
  if (state.game.status.status === "win" || state.game.status.status === "draw") loadLeaderboard().catch(reportError);
}

function reportError(error) {
  showNotice(error.message || "Что-то пошло не так.", true);
}

async function enterApp() {
  elements.authSection.classList.add("hidden");
  elements.appSection.classList.remove("hidden");
  elements.signOut.classList.remove("hidden");
  await Promise.all([loadProfile(), loadLeaderboard(), loadOpenGames()]);
  startOpenGamesPolling();
  renderBoard();
}

document.querySelector("#sign-in-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const form = new FormData(event.currentTarget);
  try {
    saveTokens(await api("/signin", { method: "POST", body: JSON.stringify(Object.fromEntries(form)) }, false));
    await enterApp();
    showNotice("Вы вошли в игру.");
  } catch (error) { reportError(error); }
});

document.querySelector("#sign-up-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const form = new FormData(event.currentTarget);
  try {
    await api("/signup", { method: "POST", body: JSON.stringify(Object.fromEntries(form)) }, false);
    showNotice("Аккаунт создан. Теперь войдите с этим логином и паролем.");
    document.querySelector("#sign-in-form [name=login]").value = form.get("login");
  } catch (error) { reportError(error); }
});

elements.signOut.addEventListener("click", () => { clearSession(); showNotice("Вы вышли из аккаунта."); });
elements.leaderboardLimit.addEventListener("change", () => loadLeaderboard().catch(reportError));
elements.copyGameId.addEventListener("click", async () => {
  if (!state.game) return;
  try {
    await navigator.clipboard.writeText(state.game.id);
    showNotice("UUID игры скопирован.");
  } catch {
    showNotice("Не удалось скопировать UUID. Его можно выделить в поле ниже.", true);
  }
});
document.querySelector("#new-computer-game").addEventListener("click", () => createGame("computer").catch(reportError));
document.querySelector("#new-player-game").addEventListener("click", () => createGame("player").catch(reportError));
document.querySelector("#refresh-games-button").addEventListener("click", () => loadOpenGames().catch(reportError));
document.querySelector("#load-game-button").addEventListener("click", () => {
  const id = elements.gameIdInput.value.trim();
  if (!id) return showNotice("Введите UUID игры.", true);
  showGame(id).catch(reportError);
});

renderBoard();
if (state.accessToken) enterApp().catch((error) => {
  clearSession();
  reportError(error);
});
