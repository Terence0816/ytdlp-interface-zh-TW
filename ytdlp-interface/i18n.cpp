#include "i18n.hpp"

#include <Windows.h>

#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace
{
	std::wstring from_utf8(std::string_view text)
	{
		if(text.empty())
			return {};

		const int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size);
		return result;
	}

	std::string to_utf8(std::wstring_view text)
	{
		if(text.empty())
			return {};

		const int size = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
		std::string result(size, '\0');
		WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size, nullptr, nullptr);
		return result;
	}

	std::string translate_regex(std::string_view input)
	{
		static const std::vector<std::pair<std::regex, std::string>> rules
		{
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - settings$)"}, "$1 - 設定"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - subtitle selection$)"}, "$1 - 字幕選擇"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - queue finished$)"}, "$1 - 佇列完成"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - format sorting info$)"}, "$1 - 格式排序資訊"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - manual selection of formats$)"}, "$1 - 手動選擇格式"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - media sections$)"}, "$1 - 媒體片段"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - JSON viewer$)"}, "$1 - JSON 檢視器"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - release notes$)"}, "$1 - 更新說明"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - custom dark color theme$)"}, "$1 - 自訂深色主題"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+) - custom light color theme$)"}, "$1 - 自訂淺色主題"},
			{std::regex{R"(^(ytdlp-interface v[\d.]+)\s+\((.+) is available\)$)"}, "$1（可更新為 $2）"},
			{std::regex{R"(^(.*) \(queue item #(\d+)\)$)"}, "$1（佇列項目 #$2）"},
			{std::regex{R"(^The URL in the clipboard is already added \(queue item #(\d+)\)\.$)"}, "剪貼簿中的網址已加入佇列（項目 #$1）。"},
			{std::regex{R"(^Download options for queue item #(\d+)$)"}, "佇列項目 #$1 的下載選項"},
			{std::regex{R"(^Start item #(\d+)$)"}, "開始項目 #$1"},
			{std::regex{R"(^Stop item #(\d+)$)"}, "停止項目 #$1"},
			{std::regex{R"(^Resume item #(\d+)$)"}, "繼續項目 #$1"},
			{std::regex{R"(^Remove item #(\d+)$)"}, "移除項目 #$1"},
			{std::regex{R"(^Open folder of item #(\d+)$)"}, "開啟項目 #$1 的資料夾"},
			{std::regex{R"(^Open file of item #(\d+)$)"}, "開啟項目 #$1 的檔案"},
			{std::regex{R"(^Set file name of item #(\d+)$)"}, "設定項目 #$1 的檔名"},
			{std::regex{R"(^Select subtitles \(([^)]+)\)$)"}, "選擇字幕（$1）"},
			{std::regex{R"(^Select videos \(([^)]+)\)$)"}, "選擇影片（$1）"},
			{std::regex{R"(^Select songs \(([^)]+)\)$)"}, "選擇歌曲（$1）"},
			{std::regex{R"(^Current default: (.*)$)"}, "目前預設：$1"},
			{std::regex{R"(^stopped \((.+)\)$)"}, "已停止（$1）"},
			{std::regex{R"(^\[live event scheduled to begin in (.+)\]$)"}, "[直播活動預計於 $1 開始]"},
			{std::regex{R"(^(.+)\s+\(current = (.+)\)$)"}, "$1（目前：$2）"},
			{std::regex{R"(^(.+)\s+\(current\)\s+\[click to see changelog\]$)"}, "$1（目前使用中）[點擊查看變更記錄]"},
			{std::regex{R"(^(.+)\s+\(current\)\s+\[click for changelog\]$)"}, "$1（目前使用中）[點擊查看變更記錄]"},
			{std::regex{R"(^(.+)\s+\(current\)$)"}, "$1（目前使用中）"},
			{std::regex{R"(^(.+) lacks a 32-bit package!$)"}, "$1 缺少 32 位元套件！"},
			{std::regex{R"(^(.+) lacks a Win7 package!$)"}, "$1 缺少 Win7 套件！"},
			{std::regex{R"re(^A preset with the name "([^"]*)" already exists\.$)re"}, "名稱「$1」的預設集已存在。"},
			{std::regex{R"re(^The preset "([^"]*)" already contains the current settings\.$)re"}, "預設集「$1」已包含目前設定。"}
		};

		const std::string text(input);
		for(const auto &[pattern, replacement] : rules)
		{
			if(std::regex_match(text, pattern))
				return std::regex_replace(text, pattern, replacement);
		}

		return text;
	}
}

std::string i18n::tr(std::string_view text)
{
	static const std::unordered_map<std::string, std::string> exact
	{
		{"Settings", "設定"},
		{"Formats", "格式"},
		{"Queue actions", "佇列操作"},
		{"Copy selected URL(s)", "複製選取的網址"},
		{"Switch view (queue/output)", "切換檢視（佇列/輸出）"},
		{"Set file name of queue item", "設定佇列項目的檔名"},
		{"Delete queue item(s)", "刪除佇列項目"},
		{"Close window", "關閉視窗"},
		{"Reset window size and position", "重設視窗大小與位置"},
		{"Download folder:", "下載資料夾："},
		{"Download rate limit:", "下載速率上限："},
		{"Chapters:", "章節："},
		{"Force keyframes at cuts", "切割處強制關鍵影格"},
		{"Embed subtitles", "內嵌字幕"},
		{"Embed thumbnail", "內嵌縮圖"},
		{"Convert audio to MP3", "將音訊轉成 MP3"},
		{"File modification time = time of writing", "檔案修改時間 = 寫入時間"},
		{"Custom arguments:", "自訂參數："},
		{"Download options", "下載選項"},
		{"Show output", "顯示輸出"},
		{"Show queue", "顯示佇列"},
		{"Start download", "開始下載"},
		{"Stop download", "停止下載"},
		{"Preferred resolution:", "偏好解析度："},
		{"Preferred video codec:", "偏好視訊編碼："},
		{"Preferred audio codec:", "偏好音訊編碼："},
		{"Preferred video container:", "偏好視訊容器："},
		{"Preferred audio container:", "偏好音訊容器："},
		{"Color theme:", "色彩主題："},
		{"Contrast:", "對比："},
		{"Output template:", "輸出模板："},
		{"Max concurrent downloads:", "最大同時下載數："},
		{"Playlist indexing:", "播放清單編號："},
		{"When browsing for the output folder, start in:", "瀏覽輸出資料夾時的起始位置："},
		{"Load cookies from browser:", "從瀏覽器載入 Cookies："},
		{"Additional options:", "額外選項："},
		{"Max number of concurrent yt-dlp instances used for getting data:", "用於取得資料的 yt-dlp 最大同時執行數："},
		{"Prefer a higher framerate", "偏好較高幀率"},
		{"Dark", "深色"},
		{"Light", "淺色"},
		{"System preference", "跟隨系統"},
		{"Start next item on lengthy processing", "長時間處理時自動開始下一個項目"},
		{"Each queue item has its own download options", "每個佇列項目使用獨立下載選項"},
		{"When stopping a queue item, automatically start the next one", "停止佇列項目時自動開始下一個"},
		{"When the program starts, automatically start processing the queue", "程式啟動時自動處理佇列"},
		{"Pad the indexed filenames with zeroes", "檔名編號補零"},
		{"Put playlists in their own folders", "播放清單使用獨立資料夾"},
		{"Program folder", "程式資料夾"},
		{"Currently selected folder", "目前選取的資料夾"},
		{"Mark these categories:", "標記這些分類："},
		{"Remove these categories:", "移除這些分類："},
		{"Use this proxy:", "使用此代理："},
		{"Snap windows to screen edges", "視窗貼齊螢幕邊緣"},
		{"No minimum width for the main window", "主視窗不限制最小寬度"},
		{"[YouTube] For 1080p, prefer the \"premium\" format with enhanced bitrate", "[YouTube] 1080p 時優先使用 enhanced bitrate 的 premium 格式"},
		{"[YouTube] Use the Android player client for video extraction", "[YouTube] 擷取影片時使用 Android 播放器用戶端"},
		{"Save queue items with \"error\" status to the settings file", "將狀態為「錯誤」的佇列項目儲存到設定檔"},
		{"Automatically remove completed items (with \"done\" status)", "自動移除已完成項目（狀態為「完成」）"},
		{"Formats window: display file sizes with exact byte value", "格式視窗：以精確位元組值顯示檔案大小"},
		{"When the main window is activated, automatically add the URL from clipboard", "主視窗啟用時自動加入剪貼簿中的網址"},
		{"Display any custom file names in the \"Media title\" column", "在「媒體標題」欄位顯示自訂檔名"},
		{"Tell yt-dlp to download with aria2c  (--downloader aria2c)", "讓 yt-dlp 使用 aria2c 下載（--downloader aria2c）"},
		{"Load cookies from file:", "從檔案載入 Cookies："},
		{"Close", "關閉"},
		{"Reset to default", "恢復預設"},
		{"Options for aria2c (--downloader-args):", "aria2c 選項（--downloader-args）："},
		{"yt-dlp", "yt-dlp"},
		{"SponsorBlock", "SponsorBlock"},
		{"Queuing", "佇列"},
		{"Interface", "介面"},
		{"Updater", "更新器"},
		{"Config presets", "設定預設集"},
		{"About", "關於"},
		{"Nana C++ GUI library", "Nana C++ GUI 函式庫"},
		{"JSON for Modern C++", "JSON for Modern C++"},
		{"Libraries used", "使用的函式庫"},
		{"Keyboard shortcuts", "鍵盤快捷鍵"},
		{"Press Ctrl+V or click here to paste and add media link", "按 Ctrl+V 或點這裡貼上並加入媒體連結"},
		{"output from yt-dlp.exe appears here\n\nright-click for options\n\ndouble-click to show queue", "yt-dlp.exe 的輸出會顯示在這裡\n\n按右鍵可查看選項\n\n按兩下可顯示佇列"},
		{"queued", "已排入佇列"},
		{"downloading", "下載中"},
		{"processing", "處理中"},
		{"started", "已開始"},
		{"done", "完成"},
		{"error", "錯誤"},
		{"skip", "略過"},
		{"stopped", "已停止"},
		{"the clipboard does not contain any text", "剪貼簿中沒有任何文字"},
		{"* multiple lines of text, make sure they're URLs *", "* 內容包含多行文字，請確認它們都是網址 *"},
		{"checking...", "檢查中..."},
		{"failed to get from GitHub!", "無法從 GitHub 取得資料！"},
		{"unable to get from GitHub", "無法從 GitHub 取得資料"},
		{"Update", "更新"},
		{"Update deno", "更新 Deno"},
		{"Update Deno", "更新 Deno"},
		{"Update yt-dlp", "更新 yt-dlp"},
		{"Update FFmpeg", "更新 FFmpeg"},
		{"Cancel", "取消"},
		{"Unpacking archive and restarting...", "正在解壓縮封存並重新啟動..."},
		{"Unpacking archive to temporary folder...", "正在將封存解壓縮到暫存資料夾..."},
		{"Copying deno.exe to yt-dlp folder...", "正在將 deno.exe 複製到 yt-dlp 資料夾..."},
		{"Deno update complete", "Deno 更新完成"},
		{"Extracting files to temporary folder...", "正在將檔案解壓縮到暫存資料夾..."},
		{"FFmpeg update complete", "FFmpeg 更新完成"},
		{"yt-dlp update complete", "yt-dlp 更新完成"},
		{"No place to put the ffmpeg files", "沒有可放置 ffmpeg 檔案的位置"},
		{"No place to put yt-dlp.exe", "沒有可放置 yt-dlp.exe 的位置"},
		{"File copy error", "檔案複製錯誤"},
		{"Custom FFmpeg folder", "自訂 FFmpeg 資料夾"},
		{"The argument has been added.", "已加入參數。"},
		{"Duplicate name", "名稱重複"},
		{"Duplicate preset", "預設集重複"},
		{"Blank name", "空白名稱"},
		{"Are you sure you want to create a preset with a blank name?", "確定要建立空白名稱的預設集嗎？"},
		{"Are you sure you want to make the preset name blank?", "確定要將預設集名稱改成空白嗎？"},
		{"Choose folder...", "選擇資料夾..."},
		{"Clear folder history", "清除資料夾歷史"},
		{"Paste", "貼上"},
		{"Select formats", "選擇格式"},
		{"View JSON data", "檢視 JSON 資料"},
		{"Refresh (reacquire data)", "重新整理（重新取得資料）"},
		{"Download sections", "下載片段"},
		{"Treat as playlist", "視為播放清單"},
		{"Split playlist (add videos to queue)", "分割播放清單（將影片加入佇列）"},
		{"Do not download", "不要下載"},
		{"Make downloadable", "允許下載"},
		{"Toggle download ability", "切換是否可下載"},
		{"Clear completed", "清除已完成"},
		{"Start all", "全部開始"},
		{"Stop all", "全部停止"},
		{"Remove this playlist", "移除此播放清單"},
		{"Remove all", "全部移除"},
		{"Start selected", "開始所選項目"},
		{"Stop selected", "停止所選項目"},
		{"Start/stop selected", "開始/停止所選項目"},
		{"Remove selected", "移除所選項目"},
		{"Refresh selected", "重新整理所選項目"},
		{"Extra columns", "額外欄位"},
		{"Website column", "網站欄位"},
		{"Favicon", "網站圖示"},
		{"Text", "文字"},
		{"When finished...", "完成後..."},
		{"Shutdown", "關機"},
		{"Hibernate", "休眠"},
		{"Sleep", "睡眠"},
		{"Exit", "結束"},
		{"Format", "格式"},
		{"Format note", "格式說明"},
		{"Ext", "副檔名"},
		{"Filesize", "檔案大小"},
		{"Website", "網站"},
		{"Media title", "媒體標題"},
		{"Status", "狀態"},
		{"ignore", "忽略"},
		{"embed", "內嵌"},
		{"split", "分割"},
		{"shutting down in:", "將於以下時間後關機："},
		{"attempting to shut down in:", "將嘗試於以下時間後關機："},
		{"hibernating in:", "將於以下時間後休眠："},
		{"attempting to hibernate in:", "將嘗試於以下時間後休眠："},
		{"sleeping in:", "將於以下時間後睡眠："},
		{"attempting to sleep in:", "將嘗試於以下時間後睡眠："},
		{" KB/s", " KB/秒"},
		{" MB/s", " MB/秒"},
		{"<any>", "<任何>"},
		{"Can't parse the JSON data produced by yt-dlp! See output for details.", "無法解析 yt-dlp 產生的 JSON 資料！詳情請見輸出內容。"},
		{"!!! failed to restore the media title !!!", "!!! 還原媒體標題失敗 !!!"},
		{"error: playlist with zero entries!", "錯誤：播放清單沒有任何項目！"},
		{"yt-dlp failed to get info (see output)", "yt-dlp 取得資訊失敗（請查看輸出）"},
		{"can't get data for URL, yt-dlp.exe missing!", "無法取得此網址的資料，缺少 yt-dlp.exe！"},
		{"yt-dlp did not provide any data for this URL!", "yt-dlp 沒有為此網址提供任何資料！"},
		{"[channel tab] ", "[頻道分頁] "},
		{"[playlist] ", "[播放清單] "},
		{"[live] ", "[直播] "},
		{"Shutting down active yt-dlp instances", "正在關閉執行中的 yt-dlp 執行個體"},
		{"Please wait...", "請稍候..."},
		{"ytdlp-interface - error writing settings file", "ytdlp-interface - 寫入設定檔錯誤"},
		{"ytdlp-interface JSON error", "ytdlp-interface JSON 錯誤"},
		{"ytdlp-interface error", "ytdlp-interface 錯誤"},
		{"ytdlp-interface update error", "ytdlp-interface 更新錯誤"},
		{"failed to open debug_log.txt for writing", "無法開啟 debug_log.txt 以進行寫入"},
		{"Show release notes for:", "顯示此版本的更新說明："},
		{"Latest version:", "最新版本："},
		{"Release notes", "更新說明"},
		{"Check at program startup and display any new version in the title bar", "程式啟動時檢查更新，並在標題列顯示新版本"},
		{"Only extract ytdlp-interface.exe from the downloaded archive", "下載更新封存時只解出 ytdlp-interface.exe"},
		{"Path to yt-dlp:", "yt-dlp 路徑："},
		{"FFmpeg folder:", "FFmpeg 資料夾："},
		{"Latest yt-dlp version:", "最新 yt-dlp 版本："},
		{"Latest FFmpeg version:", "最新 FFmpeg 版本："},
		{"Latest Deno version:", "最新 Deno 版本："},
		{"yt-dlp release channel:", "yt-dlp 發行頻道："},
		{"Stable", "穩定版"},
		{"Nightly", "每夜版"},
		{"When updating ffmpeg, also extract \"ffplay.exe\"", "更新 ffmpeg 時也一併解出「ffplay.exe」"},
		{"JavaScript runtime", "JavaScript 執行環境"},
		{"aria2c.exe found in the yt-dlp folder", "已在 yt-dlp 資料夾中找到 aria2c.exe"},
		{"aria2c.exe found in the %path% system variable", "已在系統 %path% 變數中找到 aria2c.exe"},
		{"aria2c.exe not found in the yt-dlp folder or the %path% system variable", "在 yt-dlp 資料夾或系統 %path% 變數中都找不到 aria2c.exe"},
		{"Name for new preset:", "新預設集名稱："},
		{"Create", "建立"},
		{"Save", "儲存"},
		{"Load", "載入"},
		{"Delete", "刪除"},
		{"Rename", "重新命名"},
		{"<bold>Configuration presets</> are snapshots of the settings found in the first three categories (yt-dlp, SponsorBlock, Queuing), as well as the ones in the \"Download options\" group in the main window. The \"Save\" button saves the current settings to the selected preset, and the \"Load\" button loads the selected preset into the current settings.", "<bold>設定預設集</> 會保存前三個分類（yt-dlp、SponsorBlock、佇列）以及主視窗「下載選項」群組中的設定快照。「儲存」按鈕會把目前設定存到選取的預設集，而「載入」按鈕則會把該預設集套用到目前設定。"},
		{"All", "全部"},
		{"Sponsor", "贊助內容"},
		{"Intermission/Intro Animation", "開場動畫／中場過場"},
		{"Endcards/Credits (Outro)", "結尾卡／片尾名單"},
		{"Unpaid/Self Promotion", "非付費／自我宣傳"},
		{"Preview/Recap", "預告／回顧"},
		{"Filler Tangent/Jokes", "填充閒聊／玩笑"},
		{"Interaction Reminder (Subscribe)", "互動提醒（訂閱）"},
		{"Music: Non-Music Section", "音樂影片中的非音樂片段"},
		{"Highlight", "重點片段"},
		{"Chapter", "章節"},
		{"Hook/Greetings", "開場鉤子／問候"},
		{"!!!  FFMPEG.EXE NOT FOUND IN THE PROGRAM FOLDER  !!!", "!!! 程式資料夾中找不到 FFMPEG.EXE !!!"},
		{"!!!  FFMPEG.EXE NOT FOUND IN THE SELECTED FOLDER  !!!", "!!! 在選取的資料夾中找不到 FFMPEG.EXE !!!"},
		{"!!!  YT-DLP EXECUTABLE NOT FOUND IN PROGRAM FOLDER  !!!", "!!! 程式資料夾中找不到 YT-DLP 執行檔 !!!"},
		{"!!!  YT-DLP EXECUTABLE NOT FOUND AT ITS SELECTED LOCATION  !!!", "!!! 在選定位置找不到 YT-DLP 執行檔 !!!"}
	};

	const auto it = exact.find(std::string(text));
	if(it != exact.end())
		return it->second;

	return translate_regex(text);
}

std::wstring i18n::trw(std::wstring_view text)
{
	return from_utf8(tr(to_utf8(text)));
}
