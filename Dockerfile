FROM alpine

RUN apk add --update alpine-sdk

WORKDIR /usr/yatcp-cc/

COPY . .

RUN make

EXPOSE 10023

RUN ./test.out
