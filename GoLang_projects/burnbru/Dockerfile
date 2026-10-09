FROM golang:1.26.5 AS builder

WORKDIR /app

COPY go.mod go.sum ./
RUN go mod download

COPY . .
RUN CGO_ENABLED=0 GOOS=linux go build -o /out/burnbru ./cmd

FROM alpine:3.21

WORKDIR /app

COPY --from=builder /out/burnbru /app/burnbru
COPY --from=builder /app/uploads /app/uploads

ENV PHOTO_STORAGE_DIR=/data/uploads

EXPOSE 8080

CMD ["/app/burnbru"]