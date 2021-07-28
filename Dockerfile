FROM alpine:3.14.0 AS build
RUN apk add --no-cache make gcc

COPY . /app/holyc
WORKDIR /app/holyc
RUN make

FROM scratch
COPY --from=build /app/holyc/holyc /bin/holyc

ENTRYPOINT ["/bin/holyc"]
