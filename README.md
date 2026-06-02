# ytdlp-interface 繁體中文化版
[![Release](https://img.shields.io/github/v/release/Terence0816/ytdlp-interface-zh-TW?label=Release&color=2d7d46)](https://github.com/Terence0816/ytdlp-interface-zh-TW/releases)
[![Downloads](https://img.shields.io/github/downloads/Terence0816/ytdlp-interface-zh-TW/total?label=Downloads&color=1f6feb)](https://github.com/Terence0816/ytdlp-interface-zh-TW/releases)

本專案基於 [ErrorFlynn/ytdlp-interface](https://github.com/ErrorFlynn/ytdlp-interface) `v2.19.1` 製作，提供 `ytdlp-interface` 的繁體中文化介面、建置腳本與 Windows 編譯整理，方便在既有 `yt-dlp` 工作流程中使用圖形化介面。

> 本專案為非官方繁體中文化版本，並非 [ErrorFlynn/ytdlp-interface](https://github.com/ErrorFlynn/ytdlp-interface) 原作者官方發行版本。

## 專案重點
- 主介面與設定頁面繁體中文化。
- `ytdlp-interface` 自身更新檢查改為此繁中版專案。
- `yt-dlp` 與 `FFmpeg` 更新來源維持原始上游。
- 內附 `build_zh_tw.bat`，方便在 Windows 環境重新編譯。
- Release 壓縮檔可搭配既有 `yt-dlp.exe`、`ffmpeg.exe`、`ffprobe.exe` 使用。

## 下載與使用
請前往 [Releases](https://github.com/Terence0816/ytdlp-interface-zh-TW/releases) 下載已整理的發行版本。

一般使用時，請將 `ytdlp-interface.exe` 與下列檔案放在同一資料夾：
- `yt-dlp.exe`
- `ffmpeg.exe`
- `ffprobe.exe`

## 使用提醒
本專案僅提供開源軟體介面中文化與技術測試用途，請僅處理自己擁有權利、已取得授權、公開授權，或平台規範允許保存的影音內容。使用本工具時，請自行遵守相關網站服務條款與著作權規定。

## 建置方式
### 快速建置
專案根目錄提供 `build_zh_tw.bat`，可在安裝 Visual Studio / Build Tools、MSBuild、CMake 與 Windows SDK 的環境中使用：

```bat
build_zh_tw.bat
build_zh_tw.bat Release x64
build_zh_tw.bat Debug Win32
build_zh_tw.bat Release x64 v143
```

批次檔會：
1. 載入 Visual Studio 開發環境。
2. 解壓 `ytdlp-interface dependencies.7z` 內的相依項目。
3. 建置 `nana`、`bit7z`、`libpng`、`libjpeg-turbo`。
4. 最後建置 `ytdlp-interface\ytdlp-interface.sln`。

### 手動建置
若要手動編譯，可依序處理下列元件：
- [Nana C++ GUI library](https://github.com/cnjinhao/nana) v1.8 (custom)
- [libjpeg-turbo](https://github.com/libjpeg-turbo/libjpeg-turbo)
- [libpng](http://www.libpng.org/pub/png/libpng.html)
- [bit7z](https://github.com/rikyoz/bit7z)
- [JSON for Modern C++](https://github.com/nlohmann/json)

`ytdlp-interface dependencies.7z` 內已整理建置時會用到的目錄：
- `bit7z`
- `libjpeg-turbo-3.1.2`
- `libpng`
- `nana`
- `ytdlp-interface`

相依專案可分別由以下方案或流程建置：
- `nana\build\vc2022\nana.sln`
- `bit7z\bit7z.sln`
- `libpng\libpng.sln`
- `libjpeg-turbo-3.1.2` 以 CMake 建置

最後再建置 `ytdlp-interface\ytdlp-interface.sln`。

## 第三方依賴
本專案包含或引用 Nana、libjpeg-turbo、libpng、bit7z、JSON for Modern C++ 等第三方元件。這些元件各自適用其原始授權條款，請以各上游專案授權內容為準。

## 原始專案與授權
本專案修改自 [ErrorFlynn/ytdlp-interface](https://github.com/ErrorFlynn/ytdlp-interface)。

- Upstream: [ErrorFlynn/ytdlp-interface](https://github.com/ErrorFlynn/ytdlp-interface)
- License: `MIT`
- Notice: [NOTICE.md](NOTICE.md)

本專案保留 upstream 的授權與版權聲明；繁體中文化、建置腳本與封裝調整由 Terence0816 維護。

如果未來不希望誤點 GitHub 的 fork 同步功能，也可參考這份整理：
- [安全脫離 fork network 前確認清單](docs/leave-fork-network-checklist.md)

## 介面預覽
主畫面
![主畫面](docs/screenshots/main-window.png)

---

下載器設定
![下載器設定](docs/screenshots/settings-ytdlp.png)

---

SponsorBlock 設定
![SponsorBlock 設定](docs/screenshots/settings-sponsorblock.png)

---

佇列設定
![佇列設定](docs/screenshots/settings-queue.png)

---

介面設定
![介面設定](docs/screenshots/settings-interface.png)

---

更新器設定
![更新器設定](docs/screenshots/settings-updater.png)

---

關於頁面
![關於頁面](docs/screenshots/settings-about.png)
