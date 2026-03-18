# denemOS

> A bare-metal hobby operating system written in C and Assembly.

---

## 🇹🇷 Türkçe

**denemOS**, C ve Assembly ile sıfırdan yazılmış, bare-metal çalışan bir hobi işletim sistemidir. Bu proje bir **öğrenme süreci ürünüdür** — işletim sistemi geliştirmenin temellerini kavramak amacıyla yapılmıştır ve büyük ölçüde [Claude.ai](https://claude.ai) yapay zeka asistanının yardımıyla geliştirilmiştir. Kodun nasıl çalıştığını anlamak öncelikli hedefti; mükemmel bir ürün çıkarmak değil.

### Özellikler (v0.2)

- ✅ Terminal üzerinden kullanıcı girişi alma
- ✅ Temel komut yorumlayıcı (shell)
- ✅ `ls` komutu ile dizin listeleme
- ✅ IMG disk imajı desteği (içinde `test.txt` dosyaları)
- ✅ Her versiyon için özel `run.sh` betiği ile tek komutla çalıştırma

### Yol Haritası

| Versiyon | Durum | İçerik |
|----------|-------|--------|
| v0.1 | ✅ Yayınlandı | Terminal girişi, temel sistem altyapısı |
| v0.2 | ✅ Yayınlandı | Shell, `ls` komutu, disk imajı |
| v0.3 | 🔧 Geliştiriliyor | Dosya okuma/yazma/silme/oluşturma, Linux tarzı `/bin` sistemi |

### Kurulum

Sistemi çalıştırabilmek için **Git**, **QEMU**, **make** ve **barebones-toolchain** gereklidir.

#### Git Kurulumu

**Ubuntu / Debian:**
```bash
sudo apt install git
```

**Arch Linux:**
```bash
sudo pacman -S git
```

**macOS:**
```bash
brew install git
```

**Windows:**
> [https://git-scm.com/download/win](https://git-scm.com/download/win) adresinden indirip kurabilirsiniz.

#### Repoyu İndirme

```bash
git clone https://github.com/KULLANICI_ADIN/denemOS.git
cd denemOS
```

#### Barebones Toolchain Kurulumu

Cross-compiler toolchain her versiyon klasörünün **içine** kurulmalıdır:

```bash
cd v0.1
git clone https://github.com/rm-hull/barebones-toolchain.git
cd ..

cd v0.2
git clone https://github.com/rm-hull/barebones-toolchain.git
cd ..
```

> Detaylı kurulum adımları için: [https://github.com/rm-hull/barebones-toolchain](https://github.com/rm-hull/barebones-toolchain)

#### QEMU Kurulumu

**Ubuntu / Debian:**
```bash
sudo apt update
sudo apt install qemu-system-x86
```

**Arch Linux:**
```bash
sudo pacman -S qemu
```

**macOS (Homebrew):**
```bash
brew install qemu
```

**Windows:**
> [https://www.qemu.org/download/#windows](https://www.qemu.org/download/#windows) adresinden indirip kurabilirsiniz.

#### Make Kurulumu

**Ubuntu / Debian:**
```bash
sudo apt install build-essential
```

**Arch Linux:**
```bash
sudo pacman -S base-devel
```

**macOS:**
```bash
xcode-select --install
```

### Nasıl Çalıştırılır

Her versiyon klasöründe kendi `run.sh` betiği bulunur. Tek yapman gereken:

```bash
cd v0.1
bash run.sh
```

```bash
cd v0.2
bash run.sh
```

`run.sh` derleme ve QEMU ile çalıştırma adımlarını otomatik olarak halleder.

---

## 🇬🇧 English

**denemOS** is a bare-metal hobby operating system written from scratch in C and Assembly. This project is a **learning-process product** — it was built to understand the fundamentals of OS development and was largely developed with the help of [Claude.ai](https://claude.ai) AI assistant. The primary goal was understanding how things work, not shipping a perfect product.

### Features (v0.2)

- ✅ Terminal input handling
- ✅ Basic command interpreter (shell)
- ✅ `ls` command for directory listing
- ✅ IMG disk image support (contains `test.txt` files)
- ✅ Per-version custom `run.sh` script for one-command execution

### Roadmap

| Version | Status | Contents |
|---------|--------|----------|
| v0.1 | ✅ Released | Terminal input, basic system infrastructure |
| v0.2 | ✅ Released | Shell, `ls` command, disk image |
| v0.3 | 🔧 In progress | File read/write/delete/create, Linux-style `/bin` system |

### Installation

You need **Git**, **QEMU**, **make** and **barebones-toolchain** to run the system.

#### Installing Git

**Ubuntu / Debian:**
```bash
sudo apt install git
```

**Arch Linux:**
```bash
sudo pacman -S git
```

**macOS:**
```bash
brew install git
```

**Windows:**
> Download and install from [https://git-scm.com/download/win](https://git-scm.com/download/win)

#### Cloning the Repository

```bash
git clone https://github.com/YOUR_USERNAME/denemOS.git
cd denemOS
```

#### Installing Barebones Toolchain

The cross-compiler toolchain must be installed **inside each version folder**:

```bash
cd v0.1
git clone https://github.com/rm-hull/barebones-toolchain.git
cd ..

cd v0.2
git clone https://github.com/rm-hull/barebones-toolchain.git
cd ..
```

> For detailed setup steps, see: [https://github.com/rm-hull/barebones-toolchain](https://github.com/rm-hull/barebones-toolchain)

#### Installing QEMU

**Ubuntu / Debian:**
```bash
sudo apt update
sudo apt install qemu-system-x86
```

**Arch Linux:**
```bash
sudo pacman -S qemu
```

**macOS (Homebrew):**
```bash
brew install qemu
```

**Windows:**
> Download and install from [https://www.qemu.org/download/#windows](https://www.qemu.org/download/#windows)

#### Installing Make

**Ubuntu / Debian:**
```bash
sudo apt install build-essential
```

**Arch Linux:**
```bash
sudo pacman -S base-devel
```

**macOS:**
```bash
xcode-select --install
```

### How to Run

Each version folder contains its own `run.sh` script. Just run:

```bash
cd v0.1
bash run.sh
```

```bash
cd v0.2
bash run.sh
```

`run.sh` handles both compilation and launching via QEMU automatically.

---

## 📁 Repository Structure

```
denemOS/
├── v0.1/
│   ├── run.sh         # Build & run script
│   └── ...
├── v0.2/
│   ├── run.sh         # Build & run script
│   └── ...
└── README.md
```

---

## 🤖 A Note on AI Assistance

This project was built with significant help from [Claude.ai](https://claude.ai). The goal was never to write every line independently, but to **learn by doing** — understanding boot processes, memory, shell design, and file systems through guided exploration. The next project, **nodyOS**, will be written independently.

---

## 📜 License

This project is open source. See [LICENSE](LICENSE) for details.
