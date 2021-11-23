FROM alpine:3.14.0 AS build
RUN apk add --no-cache make build-base nasm

COPY . /app/holyc
WORKDIR /app/holyc
RUN make

# # FROM scratch
# FROM alpine:3.14.0
# COPY --from=build /app/holyc/holyc /bin/holyc

# CMD ["/bin/holyc"]
