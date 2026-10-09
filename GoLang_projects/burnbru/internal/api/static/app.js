const fallbackAbout = {
  title: "BurnBru",
  text: " "
};

const AUTH_KEY = "liza-portfolio-auth";
const SESSION_STARTED_KEY = "liza-portfolio-auth-started";
const SESSION_DURATION = 60 * 60 * 1000;
const savedAuthorization = sessionStorage.getItem(AUTH_KEY) || "";
const sessionStartedAt = Number(sessionStorage.getItem(SESSION_STARTED_KEY) || 0);
const sessionIsValid = Boolean(savedAuthorization && sessionStartedAt && Date.now() - sessionStartedAt < SESSION_DURATION);

if (!sessionIsValid) {
  sessionStorage.removeItem(AUTH_KEY);
  sessionStorage.removeItem(SESSION_STARTED_KEY);
}

const state = {
  about: { ...fallbackAbout },
  contacts: { tel: "", mail: "", inst: "", tg: "" },
  photos: [],
  selectedPhotoNames: new Set(),
  isUploadingPhotos: false,
  isDeletingPhotos: false,
  authorization: sessionIsValid ? savedAuthorization : ""
};

const page = document.body.dataset.page;
const $ = function (selector) { return document.querySelector(selector); };

function setText(element, value) {
  if (element) element.textContent = value;
}

function showToast(message, isError) {
  const toast = $("#toast");
  if (!toast) return;
  window.clearTimeout(showToast.timer);
  toast.hidden = !message;
  toast.textContent = message || "";
  toast.style.background = isError ? "#72070b" : "#181817";
  if (message) showToast.timer = window.setTimeout(function () { toast.hidden = true; }, 4200);
}

async function request(path, options, requiresAuth) {
  const requestOptions = options || {};
  const headers = new Headers(requestOptions.headers || {});
  if (requiresAuth) {
    if (!state.authorization) throw new Error("Сессия закончилась. Войдите снова.");
    headers.set("Authorization", state.authorization);
  }
  const response = await fetch(path, { ...requestOptions, headers: headers });
  if (!response.ok) {
    const message = await response.text().catch(function () { return ""; });
    if (response.status === 401 && requiresAuth) clearAuthorization();
    throw new Error(message.trim() || "Не удалось выполнить запрос.");
  }
  return response;
}

function clearAuthorization() {
  state.authorization = "";
  sessionStorage.removeItem(AUTH_KEY);
  sessionStorage.removeItem(SESSION_STARTED_KEY);
}

function startSessionTimer() {
  if (!state.authorization) return;
  const startedAt = Number(sessionStorage.getItem(SESSION_STARTED_KEY) || 0);
  const remaining = SESSION_DURATION - (Date.now() - startedAt);
  const endSession = function () {
    if (!state.authorization) return;
    clearAuthorization();
    window.location.reload();
  };
  if (remaining <= 0) {
    endSession();
    return;
  }
  window.setTimeout(endSession, remaining);
  document.addEventListener("visibilitychange", function () {
    if (!document.hidden && Date.now() - startedAt >= SESSION_DURATION) endSession();
  });
}

function updateAccountLinks() {
  document.querySelectorAll("[data-account-link]").forEach(function (link) {
    link.href = state.authorization ? "#" : "/login/";
    link.textContent = state.authorization ? "Sign Out" : "Sign In";
    if (link.dataset.bound) return;
    link.dataset.bound = "true";
    link.addEventListener("click", function (event) {
      if (state.authorization) {
        event.preventDefault();
        clearAuthorization();
        window.location.reload();
      } else {
        sessionStorage.setItem("liza-return-path", window.location.pathname);
      }
    });
  });
}

function setFooterYear() {
  document.querySelectorAll("[data-year]").forEach(function (element) {
    element.textContent = new Date().getFullYear();
  });
}

function renderText(container, text) {
  if (!container) return;
  container.replaceChildren();
  const lines = String(text).replace(/\r\n?/g, "\n").split("\n");
  lines.forEach(function (line, index) {
    container.append(document.createTextNode(line));
    if (index < lines.length - 1) container.append(document.createElement("br"));
  });
}

function renderAbout() {
  const title = state.about.title.trim() || fallbackAbout.title;
  const text = typeof state.about.text === "string" && state.about.text.length ? state.about.text : fallbackAbout.text;
  setText($("#hero-title"), title);
  setText($("#about-title"), title);
  renderText($("#hero-text"), text);
  renderText($("#about-text"), text);
}

function randomPhoto(photos) {
  return photos.length ? photos[Math.floor(Math.random() * photos.length)] : null;
}

function renderHeroPhoto() {
  const image = $("#hero-image");
  if (!image) return;

  const cover = randomPhoto(state.photos);
  image.hidden = !cover;
  if (!cover) {
    image.removeAttribute("src");
    return;
  }

  image.src = cover.url;
  const setPhotoRatio = function () {
    const frame = image.closest(".home-photo");
    if (frame && image.naturalWidth && image.naturalHeight) {
      frame.style.setProperty("--home-photo-ratio", String(image.naturalWidth / image.naturalHeight));
    }
  };
  if (image.complete) setPhotoRatio();
  else image.addEventListener("load", setPhotoRatio, { once: true });
}

function readPhotoSize(photo) {
  if (photo.width && photo.height) return Promise.resolve(photo);
  return new Promise(function (resolve) {
    const probe = new Image();
    probe.addEventListener("load", function () {
      resolve({ ...photo, width: probe.naturalWidth, height: probe.naturalHeight });
    }, { once: true });
    probe.addEventListener("error", function () { resolve({ ...photo, width: 0, height: 0 }); }, { once: true });
    probe.src = photo.url;
  });
}

async function renderAboutPhoto() {
  const image = $("#about-image");
  if (!image) return;

  const measuredPhotos = await Promise.all(state.photos.map(readPhotoSize));
  const landscapePhotos = measuredPhotos.filter(function (photo) {
    return photo.width >= 1000 && photo.height >= 560 && photo.width > photo.height;
  });
  const cover = randomPhoto(landscapePhotos);
  const strip = image.closest(".about-photo-strip");
  if (strip) {
    strip.hidden = !cover;
    if (cover) strip.style.setProperty("--about-photo-ratio", String(cover.width / cover.height));
    else strip.style.removeProperty("--about-photo-ratio");
  }
  image.hidden = !cover;
  if (cover) image.src = cover.url;
  else image.removeAttribute("src");
}

function setUpAboutPhotoParallax() {
  const strip = $(".about-photo-strip");
  const image = $("#about-image");
  if (!strip || !image || window.matchMedia("(prefers-reduced-motion: reduce)").matches) return;

  let frame = 0;
  const update = function () {
    frame = 0;
    const bounds = strip.getBoundingClientRect();
    const distanceFromCenter = bounds.top + bounds.height / 2 - window.innerHeight / 2;
    const shift = Math.max(-32, Math.min(32, -distanceFromCenter * 0.09));
    image.style.setProperty("--scroll-shift", shift.toFixed(1) + "px");
  };
  const requestUpdate = function () {
    if (!frame) frame = window.requestAnimationFrame(update);
  };

  window.addEventListener("scroll", requestUpdate, { passive: true });
  window.addEventListener("resize", requestUpdate);
  update();
}

function linkFor(type, value) {
  const clean = value.trim();
  if (type === "Phone") return "tel:" + clean.replace(/[^\d+]/g, "");
  if (type === "Mail") return "mailto:" + clean;
  if (/^https?:\/\//i.test(clean)) return clean;
  const name = clean.replace(/^@/, "");
  return type === "Instagram" ? "https://www.instagram.com/" + encodeURIComponent(name) : "https://t.me/" + encodeURIComponent(name);
}

function renderContacts() {
  const container = $("#contact-list");
  if (!container) return;
  const contacts = [
    ["Phone", state.contacts.tel],
    ["Mail", state.contacts.mail],
    ["Instagram", state.contacts.inst],
    ["Telegram", state.contacts.tg]
  ].filter(function (item) { return item[1] && item[1].trim(); });
  container.replaceChildren();
  if (!contacts.length) {
    const note = document.createElement("p");
    note.className = "contact-empty";
    note.textContent = "Контакты появятся здесь совсем скоро.";
    container.append(note);
    return;
  }
  contacts.forEach(function (item) {
    const link = document.createElement("a");
    link.className = "contact-item";
    link.href = linkFor(item[0], item[1]);
    if (item[0] === "Instagram" || item[0] === "Telegram") {
      link.target = "_blank";
      link.rel = "noreferrer";
    }
    const label = document.createElement("span");
    const value = document.createElement("strong");
    label.textContent = item[0];
    value.textContent = item[1];
    link.append(label, value);
    container.append(link);
  });
}

function openPhoto(photo) {
  const dialog = $("#lightbox");
  const image = $("#lightbox-image");
  if (!dialog || !image) return;
  const scrollLeft = window.scrollX;
  const scrollTop = window.scrollY;
  image.src = photo.url;
  image.alt = "Фотография из портфолио";
  dialog.showModal();
  window.requestAnimationFrame(function () { window.scrollTo(scrollLeft, scrollTop); });
}

let galleryRenderVersion = 0;

function renderGallery(limit) {
  const container = $("#photo-grid");
  if (!container) return;
  const version = ++galleryRenderVersion;
  container.setAttribute("aria-busy", "true");
  const count = $("#gallery-count");
  if (count) count.hidden = true;
  const measured = state.photos.slice();
  const orientation = function (photo) {
    if (!photo.width || !photo.height) return 3;
    return photo.width > photo.height ? 0 : photo.width === photo.height ? 1 : 2;
  };
  measured.sort(function (a, b) { return orientation(a) - orientation(b); });
  const photos = limit ? measured.slice(0, limit) : measured;
  container.replaceChildren();
  container.setAttribute("aria-busy", "false");
  if (!photos.length) {
    const empty = document.createElement("div");
    empty.className = "gallery-empty";
    empty.innerHTML = "<div><strong>Footage will be available here soon</strong></div>";
    container.append(empty);
    return;
  }
  let group;
  let previousOrientation = -1;
  photos.forEach(function (photo) {
    const currentOrientation = orientation(photo);
    if (currentOrientation !== previousOrientation) {
      group = document.createElement("div");
      group.className = "photo-orientation-group";
      container.append(group);
      previousOrientation = currentOrientation;
    }
    const button = document.createElement("button");
    button.className = "photo-card";
    button.type = "button";
    button.setAttribute("aria-label", "Открыть фотографию");
    const image = document.createElement("img");
    image.src = photo.url;
    image.alt = "Фотография из портфолио";
    image.loading = "lazy";
    if (photo.width && photo.height) {
      image.width = photo.width;
      image.height = photo.height;
    }
    image.addEventListener("error", function () { button.hidden = true; });
    button.append(image);
    button.addEventListener("click", function () { openPhoto(photo); });
    group.append(button);
  });
}

async function loadContent() {
  const results = await Promise.allSettled([
    request("/about").then(function (response) { return response.json(); }),
    request("/contacts").then(function (response) { return response.json(); }),
    request("/photo").then(function (response) { return response.json(); })
  ]);
  if (results[0].status === "fulfilled" && results[0].value) state.about = { ...fallbackAbout, ...results[0].value };
  if (results[1].status === "fulfilled" && results[1].value) state.contacts = { ...state.contacts, ...results[1].value };
  if (results[2].status === "fulfilled" && Array.isArray(results[2].value)) state.photos = results[2].value;
  return results;
}

function fillEditor() {
  const aboutForm = $("#about-form");
  const contactsForm = $("#contacts-form");
  if (aboutForm) {
    aboutForm.elements.title.value = state.about.title;
    aboutForm.elements.text.value = state.about.text;
  }
  if (contactsForm) {
    contactsForm.elements.tel.value = state.contacts.tel;
    contactsForm.elements.mail.value = state.contacts.mail;
    contactsForm.elements.inst.value = state.contacts.inst;
    contactsForm.elements.tg.value = state.contacts.tg;
  }
}

function renderAdminPhotos() {
  const container = $("#admin-photo-list");
  if (!container) return;
  container.replaceChildren();
  state.selectedPhotoNames.forEach(function (filename) {
    if (!state.photos.some(function (photo) { return photo.filename === filename; })) state.selectedPhotoNames.delete(filename);
  });
  if (!state.photos.length) return;

  const toolbar = document.createElement("div");
  toolbar.className = "admin-photo-toolbar";
  const selectAllLabel = document.createElement("label");
  selectAllLabel.className = "photo-select-all";
  const selectAll = document.createElement("input");
  selectAll.type = "checkbox";
  selectAll.checked = state.photos.every(function (photo) { return state.selectedPhotoNames.has(photo.filename); });
  selectAll.indeterminate = !selectAll.checked && state.selectedPhotoNames.size > 0;
  const selectAllText = document.createElement("span");
  selectAllText.textContent = "Выбрать все";
  selectAllLabel.append(selectAll, selectAllText);
  selectAll.addEventListener("change", function () {
    state.selectedPhotoNames = selectAll.checked ? new Set(state.photos.map(function (photo) { return photo.filename; })) : new Set();
    renderAdminPhotos();
  });
  const removeSelected = document.createElement("button");
  removeSelected.type = "button";
  removeSelected.className = "delete-selected-button";
  removeSelected.disabled = !state.selectedPhotoNames.size || state.isDeletingPhotos;
  removeSelected.textContent = state.selectedPhotoNames.size ? "Удалить выбранные: " + state.selectedPhotoNames.size : "Удалить выбранные";
  removeSelected.addEventListener("click", deleteSelectedPhotos);
  toolbar.append(selectAllLabel, removeSelected);
  container.append(toolbar);

  state.photos.forEach(function (photo) {
    const row = document.createElement("div");
    row.className = "admin-photo";
    const selectLabel = document.createElement("label");
    selectLabel.className = "photo-select";
    const select = document.createElement("input");
    select.type = "checkbox";
    select.checked = state.selectedPhotoNames.has(photo.filename);
    select.setAttribute("aria-label", "Выбрать фотографию " + photo.filename);
    select.addEventListener("change", function () {
      if (select.checked) state.selectedPhotoNames.add(photo.filename);
      else state.selectedPhotoNames.delete(photo.filename);
      renderAdminPhotos();
    });
    selectLabel.append(select);
    const image = document.createElement("img");
    image.src = photo.url;
    image.alt = "";
    const name = document.createElement("span");
    name.className = "admin-photo-name";
    name.textContent = photo.filename;
    const remove = document.createElement("button");
    remove.type = "button";
    remove.className = "delete-button";
    remove.textContent = "Удалить";
    remove.addEventListener("click", function () { deletePhoto(photo); });
    row.append(selectLabel, image, name, remove);
    container.append(row);
  });
}

async function saveAbout(event) {
  event.preventDefault();
  const form = new FormData(event.currentTarget);
  const payload = { title: String(form.get("title") || "").trim(), text: String(form.get("text") || "") };
  try {
    await request("/about", { method: "PUT", headers: { "Content-Type": "application/json" }, body: JSON.stringify(payload) }, true);
    state.about = { ...fallbackAbout, ...payload };
    renderAbout();
    closeEditors();
    showToast("Текст сохранён.");
  } catch (error) { showToast(error.message, true); }
}

async function saveContacts(event) {
  event.preventDefault();
  const form = new FormData(event.currentTarget);
  const payload = {
    tel: String(form.get("tel") || "").trim(), mail: String(form.get("mail") || "").trim(),
    inst: String(form.get("inst") || "").trim(), tg: String(form.get("tg") || "").trim()
  };
  try {
    await request("/contacts", { method: "PUT", headers: { "Content-Type": "application/json" }, body: JSON.stringify(payload) }, true);
    state.contacts = payload;
    renderContacts();
    closeEditors();
    showToast("Контакты сохранены.");
  } catch (error) { showToast(error.message, true); }
}

function formatFileSize(bytes) {
  if (bytes < 1024 * 1024) return Math.ceil(bytes / 1024) + " КБ";
  return (bytes / (1024 * 1024)).toFixed(1) + " МБ";
}

function setUploadProgress(percent, message) {
  const progress = $("#upload-progress");
  const bar = $("#upload-progress-bar");
  const text = $("#upload-progress-text");
  const value = $("#upload-progress-value");
  if (!progress || !bar || !text || !value) return;
  progress.hidden = false;
  bar.style.width = percent + "%";
  const track = $("#upload-progress [role=progressbar]");
  if (track) track.setAttribute("aria-valuenow", String(percent));
  text.textContent = message;
  value.textContent = percent + "%";
}

function chunkRequest(method, url, body, onProgress) {
  return new Promise(function (resolve, reject) {
    if (!state.authorization) { reject(new Error("Сессия закончилась. Войдите снова.")); return; }
    const xhr = new XMLHttpRequest();
    xhr.open(method, url, true);
    xhr.timeout = 60000;
    xhr.setRequestHeader("Authorization", state.authorization);
    xhr.setRequestHeader("Content-Type", typeof body === "string" ? "application/json" : "application/octet-stream");
    const fail = function (message, retryable) {
      const error = new Error(message);
      error.retryable = retryable;
      reject(error);
    };
    xhr.upload.addEventListener("progress", function (event) {
      if (event.lengthComputable && onProgress) onProgress(event.loaded, event.total);
    });
    xhr.addEventListener("load", function () {
      if (xhr.status >= 200 && xhr.status < 300) { resolve(); return; }
      if (xhr.status === 401) clearAuthorization();
      fail("HTTP " + xhr.status + ": " + (xhr.responseText.trim().slice(0, 300) || "Ошибка загрузки"),
        xhr.status === 408 || xhr.status === 429 || xhr.status >= 500);
    });
    xhr.addEventListener("error", function () { fail("Соединение потеряно", true); });
    xhr.addEventListener("timeout", function () { fail("Истекло время ожидания ответа", true); });
    xhr.addEventListener("abort", function () { fail("Запрос отменён", false); });
    xhr.send(body);
  });
}

async function retryChunkRequest(method, url, body, onProgress) {
  for (let attempt = 0; ; attempt += 1) {
    try { return await chunkRequest(method, url, body, onProgress); }
    catch (error) {
      if (!error.retryable || attempt >= 3 || !state.authorization) throw error;
      await new Promise(function (resolve) { setTimeout(resolve, 1000 * Math.pow(2, attempt)); });
    }
  }
}

async function uploadWithProgress(file, onProgress) {
  const id = crypto.randomUUID();
  const base = "/photo/uploads/" + id;
  await retryChunkRequest("PUT", base, JSON.stringify({ filename: file.name, size: file.size }));
  const chunkSize = 9 * 1024 * 1024;
  for (let offset = 0, index = 0; offset < file.size; offset += chunkSize, index += 1) {
    const chunk = file.slice(offset, Math.min(offset + chunkSize, file.size));
    await retryChunkRequest("PUT", base + "/chunks/" + index, chunk, function (loaded, total) {
      onProgress(offset + chunk.size * Math.min(1, loaded / total), file.size);
    });
    onProgress(Math.min(offset + chunkSize, file.size), file.size);
  }
  try {
    await retryChunkRequest("POST", base + "/complete", null);
  } catch (error) {
    throw new Error(error.message + ". Сохранение не подтверждено: проверьте галерею перед повторным выбором файла.");
  }
}

async function uploadSelectedPhotos(formElement) {
  if (state.isUploadingPhotos) return;
  const input = $("#photos-form input[type=file]");
  const files = Array.from(input.files || []);
  if (!files.length) return;
  const doneButton = $("#photos-done");
  const closeButton = $("#photos-close");
  const totalBytes = files.reduce(function (total, file) { return total + file.size; }, 0);
  let uploadedBytes = 0;
  let completed = 0;
  const failures = [];
  let report = $("#upload-errors");
  if (!report) {
    report = document.createElement("pre");
    report.id = "upload-errors";
    report.style.whiteSpace = "pre-wrap";
    report.setAttribute("role", "status");
    formElement.appendChild(report);
  }
  report.textContent = "";
  state.isUploadingPhotos = true;
  input.disabled = true;
  if (doneButton) doneButton.disabled = true;
  if (closeButton) closeButton.disabled = true;
  setText($("#file-name"), "Загрузка началась. Не обновляйте страницу.");
  try {
    for (let index = 0; index < files.length; index += 1) {
      const file = files[index];
      setUploadProgress(totalBytes ? Math.floor(uploadedBytes / totalBytes * 100) : 0,
        "Фото " + (index + 1) + " из " + files.length + ": " + file.name);
      try {
        if (!/\.(jpe?g|png|webp|gif)$/i.test(file.name)) throw new Error("Поддерживаются JPG, PNG, WEBP и GIF.");
        if (!file.size || file.size > 32 * 1024 * 1024) throw new Error("Размер фото должен быть от 1 байта до 32 МиБ.");
        await uploadWithProgress(file, function (loaded, total) {
          // Progress is measured in original file bytes across all chunks.
          const currentBytes = uploadedBytes + file.size * Math.min(1, loaded / total);
          const percent = totalBytes ? Math.min(99, Math.floor(currentBytes / totalBytes * 100)) : 0;
          setUploadProgress(percent, "Фото " + (index + 1) + " из " + files.length +
            ": " + file.name + (loaded === total ? ". Ожидаем сохранения…" : ". Передаём…"));
        });
        uploadedBytes += file.size;
        completed += 1;
      } catch (error) {
        failures.push(file.name + ": " + error.message);
        report.textContent = "Не подтверждена загрузка:\n" + failures.join("\n");
        if (!state.authorization) {
          report.textContent += "\nВойдите снова. Не отправлено оставшихся файлов: " + (files.length - index - 1);
          break;
        }
      }
    }
    const percent = completed === files.length ? 100 : (totalBytes ? Math.floor(uploadedBytes / totalBytes * 100) : 0);
    setUploadProgress(percent, "Сохранено " + completed + " из " + files.length + " фото (" + formatFileSize(uploadedBytes) + ").");
    setText($("#file-name"), failures.length ? "Список ошибок ниже. Можно выбрать проблемные файлы повторно." : "Фотографии добавлены.");
    try {
      state.photos = await (await request("/photo")).json();
      renderAdminPhotos();
      renderGallery();
    } catch (error) {
      report.textContent += "\nНе удалось обновить галерею: " + error.message;
    }
  } finally {
    formElement.reset();
    state.isUploadingPhotos = false;
    input.disabled = false;
    if (doneButton) doneButton.disabled = false;
    if (closeButton) closeButton.disabled = false;
  }
}

async function deletePhoto(photo) {
  if (!window.confirm("Удалить эту фотографию из галереи?")) return;
  try {
    await request("/photo/" + encodeURIComponent(photo.filename), { method: "DELETE" }, true);
    state.photos = state.photos.filter(function (item) { return item.filename !== photo.filename; });
    state.selectedPhotoNames.delete(photo.filename);
    renderAdminPhotos();
    renderGallery();
    showToast("Фотография удалена.");
  } catch (error) { showToast(error.message, true); }
}

async function deleteSelectedPhotos() {
  const filenames = Array.from(state.selectedPhotoNames);
  if (!filenames.length || state.isDeletingPhotos) return;
  if (!window.confirm("Удалить выбранные фотографии: " + filenames.length + "?")) return;
  state.isDeletingPhotos = true;
  renderAdminPhotos();
  const results = await Promise.allSettled(filenames.map(function (filename) {
    return request("/photo/" + encodeURIComponent(filename), { method: "DELETE" }, true);
  }));
  const deleted = filenames.filter(function (_, index) { return results[index].status === "fulfilled"; });
  const errors = results.length - deleted.length;
  state.photos = state.photos.filter(function (photo) { return !deleted.includes(photo.filename); });
  state.selectedPhotoNames = new Set();
  state.isDeletingPhotos = false;
  renderAdminPhotos();
  renderGallery();
  showToast(errors ? "Удалено: " + deleted.length + ". Не удалось удалить: " + errors + "." : "Фотографии удалены: " + deleted.length + ".", Boolean(errors));
}

function setUpMenu() {
  const button = $(".menu-toggle");
  const nav = $("#site-nav");
  if (!button || !nav) return;
  button.addEventListener("click", function () {
    const open = button.getAttribute("aria-expanded") === "true";
    button.setAttribute("aria-expanded", String(!open));
    nav.classList.toggle("is-open", !open);
  });
}

async function setUpLogin() {
  if (state.authorization) {
    window.location.replace(sessionStorage.getItem("liza-return-path") || "/");
    return;
  }
  const form = $("#login-form");
  form.addEventListener("submit", async function (event) {
    event.preventDefault();
    const values = new FormData(form);
    const login = String(values.get("login") || "");
    const password = String(values.get("password") || "");
    const basic = "Basic " + btoa(unescape(encodeURIComponent(login + ":" + password)));
    const error = $("#login-error");
    error.textContent = "";
    try {
      await request("/signin", { method: "POST", headers: { Authorization: basic } });
      state.authorization = basic;
      sessionStorage.setItem(AUTH_KEY, basic);
      sessionStorage.setItem(SESSION_STARTED_KEY, String(Date.now()));
      const destination = sessionStorage.getItem("liza-return-path") || "/";
      sessionStorage.removeItem("liza-return-path");
      window.location.assign(destination);
    } catch (requestError) {
      error.textContent = "Проверьте логин и пароль.";
    }
  });
}

async function setUpAdmin() {
  window.location.replace("/");
}

function closeEditors() {
  document.querySelectorAll(".inline-editor, .gallery-editor").forEach(function (form) { form.hidden = true; });
  const about = $("#about-text");
  if (about) about.hidden = false;
}

function setUpPublicEditors() {
  document.querySelectorAll("[data-edit-control]").forEach(function (button) {
    button.hidden = !state.authorization;
  });
  if (!state.authorization) return;

  fillEditor();
  renderAdminPhotos();
  const aboutButton = $("[data-open-about-editor]");
  const contactsButton = $("[data-open-contacts-editor]");
  const galleryButton = $("[data-open-gallery-editor]");
  const aboutForm = $("#about-form");
  const contactsForm = $("#contacts-form");
  const photosForm = $("#photos-form");

  if (aboutButton && aboutForm) aboutButton.addEventListener("click", function () {
    fillEditor();
    $("#about-text").hidden = true;
    aboutForm.hidden = false;
  });
  if (contactsButton && contactsForm) contactsButton.addEventListener("click", function () { contactsForm.hidden = false; });
  if (galleryButton && photosForm) galleryButton.addEventListener("click", function () { photosForm.hidden = false; });
  if (aboutForm) aboutForm.addEventListener("submit", saveAbout);
  if (contactsForm) contactsForm.addEventListener("submit", saveContacts);
  if (photosForm) {
    const input = $("#photos-form input[type=file]");
    input.addEventListener("change", function (event) {
      const count = event.currentTarget.files.length;
      if (!count) {
        setText($("#file-name"), "JPG, PNG, WEBP или GIF");
        return;
      }
      uploadSelectedPhotos(photosForm);
    });
  }
  document.querySelectorAll("[data-close-editor]").forEach(function (button) {
    button.addEventListener("click", closeEditors);
  });
}

async function startPublicPage() {
  const results = await loadContent();
  renderAbout();
  renderContacts();
  renderHeroPhoto();
  renderAboutPhoto();
  setUpAboutPhotoParallax();
  if (page === "home") renderGallery(3);
  if (page === "gallery") renderGallery();
  setUpPublicEditors();
  if (results.some(function (result) { return result.status === "rejected"; })) setText($("#gallery-status"), " ");
  const lightbox = $("#lightbox");
  if (lightbox) {
    lightbox.addEventListener("click", function (event) {
      if (event.target !== $("#lightbox-image")) lightbox.close();
    });
  }
}

setFooterYear();
updateAccountLinks();
startSessionTimer();
setUpMenu();
if (page === "login") setUpLogin();
else if (page === "admin") setUpAdmin();
else startPublicPage();
