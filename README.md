
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: Gabriel Martins Ribeiro

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, Junho de 2025

---

# Tarefa — Roteiro de FreeRTOS #1 · EmbarcaTech 2025  
Sistema multitarefa em C para **BitDogLab (RP2040)** demonstrando o uso básico do **FreeRTOS**: três tarefas controlam um LED RGB, um buzzer piezoelétrico e dois botões físicos.
---

## ✨ Funcionalidades

| Tarefa      | Descrição                                                    | Prioridade |
|-------------|--------------------------------------------------------------|------------|
| **LED RGB** | Alterna R → G → B a cada 500 ms                              | 2 |
| **Buzzer**  | Bip de 100 ms a cada 1 s                                     | 2 |
| **Botões**  | **A** suspende/retoma LED · **B** suspende/retoma Buzzer     | 3 |

---

## 🛠️ Requisitos

* Windows 10/11 + **Git Bash**
* **ARM GNU Toolchain** ≥ 14 (`arm-none-eabi-gcc`)
* **CMake** ≥ 3.22
* **VS Code** (extensões C/C++, CMake Tools)
* Submódulos Git: [`pico-sdk`](https://github.com/raspberrypi/pico-sdk) e [`FreeRTOS-Kernel`](https://github.com/FreeRTOS/FreeRTOS-Kernel)

---

## 📁 Estrutura
````text
.
├── CMakeLists.txt
├── pico_sdk_import.cmake
├── pico-sdk/            ← submódulo
├── FreeRTOS/            ← submódulo
├── src/
│   └── main.c
└── include/
    └── FreeRTOSConfig.h
````


---

## 🚀 Compilar e gravar

```bash
git submodule update --init --recursive   # baixar SDK + FreeRTOS
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
make -j4                                  # gera bitdoglab_rtos.uf2
```
1. **Segure `BOOTSEL`**, conecte a Pico e solte.  
2. Copie o arquivo **`bitdoglab_rtos.uf2`** para a unidade **RPI-RP2**.
---
## 📌 Pinos (BitDogLab)

| Função   | GPIO | Direção      |
|----------|------|--------------|
| LED R    | 13   | OUT          |
| LED G    | 11   | OUT          |
| LED B    | 12   | OUT          |
| Buzzer   | 10   | OUT          |
| Botão A  | 5    | IN + pull-up |
| Botão B  | 6    | IN + pull-up |
---
## 🧪 Teste

| Ação              | Resultado                          |
|-------------------|------------------------------------|
| Nada pressionado  | LED alterna cores; buzzer bipa     |
| Segurar **A**     | LED pausa; soltar retoma           |
| Segurar **B**     | Buzzer silencia; soltar retoma     |

---
### ➤ O que acontece se todas as tarefas tiverem a mesma prioridade?

* **Escalonamento round-robin por _time-slice_:**  
  Tarefas na mesma prioridade entram na mesma fila de pronto. Quando a tarefa atual bloqueia ou expira o quantum, o escalonador entrega a CPU à próxima da fila.

* **Fatia de tempo igualitária:**  
  Todas recebem quantidades de CPU parecidas, salvo diferenças de tempo dormindo (`vTaskDelay`) ou bloqueando.

---

### ➤ Qual tarefa consome mais tempo de CPU?

* **Depende do tempo que cada uma passa acordada.**  
  Exemplo prático do projeto:  
  | Tarefa | `vTaskDelay` | Acorda | Tendência de uso de CPU |
  |--------|--------------|--------|-------------------------|
  | Botões | 100 ms       | 10 ×/s | **Maior uso** |
  | LED    | 500 ms       | 2 ×/s  | Médio |
  | Buzzer | 900 ms       | 1 ×/s  | Menor |

  A tarefa **Botões** executa com mais frequência e, portanto, gasta mais tempo total de CPU.

---

### ➤ Riscos de _polling_ sem prioridades

1. **Baixa responsividade** – pressões rápidas podem ser perdidas.  
2. **Starvation** – laço de polling apertado pode monopolizar a CPU.  
3. **Maior consumo de energia** – CPU ocupada quando nada acontece.  
4. **Imprevisibilidade temporal** – atrasos variáveis violam requisitos de tempo-real.


---
## 📜 Licença
GNU GPL-3.0.

