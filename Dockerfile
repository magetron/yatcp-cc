FROM ubuntu

RUN apt update && apt install -y build-essential clang

WORKDIR /usr/yatcp-cc/

COPY . .

RUN make

EXPOSE 10023

ENV LD_LIBRARY_PATH /usr/yatcp-cc

CMD ./test.out
