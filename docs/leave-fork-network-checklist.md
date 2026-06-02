# 安全脫離 fork network 前的確認清單

這份清單是給 `Terence0816/ytdlp-interface-zh-TW` 用的。目標是在不誤刪成果的前提下，把 GitHub 上的 fork 倉庫轉成獨立倉庫，避免之後手滑按到 `Sync fork`。

官方說明：

- [Detaching a fork - GitHub Docs](https://docs.github.com/pull-requests/collaborating-with-pull-requests/working-with-forks/detaching-a-fork?platform=windows)
- [Syncing a fork - GitHub Docs](https://docs.github.com/pull-requests/collaborating-with-pull-requests/working-with-forks/syncing-a-fork?lang=en)

## 先確認能不能直接用 GitHub 的 `Leave fork network`

- 倉庫是 `public`
- 倉庫小於 `1 GB`
- 沒有子 fork 掛在你這個 repo 底下

只要其中一項不符合，就不能直接用 `Leave fork network`，必須改走「備份後重建倉庫」那條路。

## 操作前一定要先做的事

- 確認 `main` 已經推上 GitHub，而且內容就是你要保留的最新版
- 確認 Release、標籤、附件都已經建立完成
- 再抓一份本地完整備份，至少保留目前這個 repo 工作目錄和發行壓縮檔
- 記下目前 Release 網址與下載檔案名稱，避免之後比對不到
- 確認你沒有還要保留的 fork 關聯功能，例如既有 PR 關聯或 fork network 顯示

## 你要知道的風險

- `Leave fork network` 是不可逆的
- GitHub 官方明確表示，離開 fork network 後，fork 關聯中的一些中繼資料不會保留
- 如果改走「刪掉 fork 再重建同名 repo」的方式，風險更高，因為中間有刪除步驟

## 對你目前這個 repo 的建議順序

1. 先確認 [v2.19.1 Release](https://github.com/Terence0816/ytdlp-interface-zh-TW/releases/tag/v2.19.1) 和下載檔都正常。
2. 在 GitHub 網頁打開 repo 的 `Settings`。
3. 進入 `General`，往下找到 `Danger Zone`。
4. 如果看得到 `Leave fork network`，優先走這個官方流程，不要先刪 repo。
5. 讀完警告後，再輸入 repo 名稱確認。
6. 完成後重新整理 repo 頁面，確認不再顯示 `forked from ErrorFlynn/ytdlp-interface`。
7. 最後再測一次 README、Releases、下載連結是否都正常。

## 如果你想把風險再降一點

- 先把目前 repo 再 mirror 一份到本地或其他私人備份位置
- 操作前截圖 repo 首頁、Releases 頁、Settings 頁
- 完成後先不要立刻做大改，先確認 fork 標記真的消失

## 什麼情況下先不要動

- 你還沒確認 Release 下載檔是否正確
- 你近期還想拿這個 repo 和上游做 PR 對照
- 你還沒有做本地或離線備份
