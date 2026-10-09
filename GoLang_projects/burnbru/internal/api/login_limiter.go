package api

import (
	"net"
	"net/http"
	"strconv"
	"strings"
	"sync"
	"time"
)

const (
	maxFailedLogins = 5
	blockDuration   = 15 * time.Minute
	failureWindow   = 15 * time.Minute
	cleanupInterval = 10 * time.Minute
)

type loginAttempt struct {
	failures    int
	lastFailure time.Time
	blockedTo   time.Time
}

type LoginLimiter struct {
	mu          sync.Mutex
	attempts    map[string]loginAttempt
	lastCleanup time.Time
}

func NewLoginLimiter() *LoginLimiter {
	return &LoginLimiter{attempts: make(map[string]loginAttempt)}
}

func clientIP(r *http.Request) string {
	host, _, err := net.SplitHostPort(r.RemoteAddr)
	if err != nil {
		return r.RemoteAddr
	}
	if host == "127.0.0.1" || host == "::1" {
		forwarded := strings.TrimSpace(strings.Split(r.Header.Get("X-Forwarded-For"), ",")[0])
		if net.ParseIP(forwarded) != nil {
			return forwarded
		}
	}
	return host
}

func (limiter *LoginLimiter) IsBlocked(ip string) (time.Duration, bool) {
	limiter.mu.Lock()
	defer limiter.mu.Unlock()

	now := time.Now()
	limiter.cleanupLocked(now)
	attempt, exists := limiter.attempts[ip]
	if !exists || attempt.blockedTo.IsZero() {
		return 0, false
	}

	remaining := time.Until(attempt.blockedTo)
	if remaining <= 0 {
		delete(limiter.attempts, ip)
		return 0, false
	}

	return remaining, true
}

func (limiter *LoginLimiter) RegisterFailure(ip string) {
	limiter.mu.Lock()
	defer limiter.mu.Unlock()

	now := time.Now()
	limiter.cleanupLocked(now)
	attempt := limiter.attempts[ip]
	if !attempt.lastFailure.IsZero() && now.Sub(attempt.lastFailure) > failureWindow {
		attempt = loginAttempt{}
	}

	attempt.failures++
	attempt.lastFailure = now
	if attempt.failures >= maxFailedLogins {
		attempt.blockedTo = now.Add(blockDuration)
		attempt.failures = 0
	}
	limiter.attempts[ip] = attempt
}

func (limiter *LoginLimiter) RegisterSuccess(ip string) {
	limiter.mu.Lock()
	defer limiter.mu.Unlock()

	delete(limiter.attempts, ip)
}

func (limiter *LoginLimiter) cleanupLocked(now time.Time) {
	if !limiter.lastCleanup.IsZero() && now.Sub(limiter.lastCleanup) < cleanupInterval {
		return
	}
	for ip, attempt := range limiter.attempts {
		if (!attempt.blockedTo.IsZero() && now.After(attempt.blockedTo)) || (!attempt.lastFailure.IsZero() && now.Sub(attempt.lastFailure) > failureWindow) {
			delete(limiter.attempts, ip)
		}
	}
	limiter.lastCleanup = now
}

func writeBlockedResponse(w http.ResponseWriter, remaining time.Duration) {
	seconds := int64(remaining.Seconds())
	if seconds < 1 {
		seconds = 1
	}
	w.Header().Set("Retry-After", strconv.FormatInt(seconds, 10))
	http.Error(w, "Too many login attempts. Try again later.", http.StatusTooManyRequests)
}
