FROM ubuntu:20.04 AS build
RUN apt-get update -y && apt-get install -y make nasm lldb mandoc build-essential

COPY . /app/holyc
WORKDIR /app/holyc
RUN make

# # FROM scratch
# FROM alpine:3.14.0
# COPY --from=build /app/holyc/holyc /bin/holyc

# CMD ["/bin/holyc"]
# CMD ["make"]
