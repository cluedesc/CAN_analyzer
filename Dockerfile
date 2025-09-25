FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    clang \
    git \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    libglfw3-dev \
    libpq-dev \
    libpqxx-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/jtv/libpqxx.git /tmp/libpqxx \
    && mkdir -p /tmp/libpqxx/build \
    && cd /tmp/libpqxx/build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DSKIP_BUILD_TEST=ON \
    && make -j$(nproc) \
    && make install \
    && ldconfig \
    && rm -rf /tmp/libpqxx

WORKDIR /opt/CAN_analyzer

COPY . ./

RUN mkdir build
WORKDIR /opt/CAN_analyzer/build

RUN cmake .. && make -j$(nproc)

RUN if [ ! -f ./CAN_analyzer ]; then echo "Build failed" && exit 1; fi

CMD ["echo", "Build successful"]