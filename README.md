
# CAN_analyzer

Инструмент для анализа CAN-трафика.

---

## Сборка и установка (Ubuntu 24.04)

### 1. Установка зависимостей

```bash
sudo apt update

sudo apt install -y build-essential cmake clang libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglfw3-dev libpq-dev libpqxx-dev
```

### 1.1 Установка libpqxx

```bash
git clone https://github.com/jtv/libpqxx.git
cd libpqxx

mkdir build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release -DSKIP_BUILD_TEST=ON

make -j$(nproc)

sudo make install
```

---

### 2. Сборка CAN_analyzer

```bash
git clone --recurse-submodules https://github.com/cluedesch/CAN_analyzer.git
cd CAN_analyzer

mkdir build && cd build

cmake ..

make -j$(nproc)
```

---

### 3. Запуск приложения

```bash
./CAN_analyzer
```

#### 3.1 Запуск с пользовательским размером окна

```bash
./CAN_analyzer --width 1280 --height 720
```