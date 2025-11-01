# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**AyuGram Desktop** is a feature-rich fork of Telegram Desktop that adds privacy-focused enhancements, extensive customization options, and productivity features. The codebase is written in C++17 using Qt6 and follows Telegram Desktop's modular architecture with all custom features cleanly separated in the `Telegram/SourceFiles/ayu/` directory.

## Build Commands

### Windows (Visual Studio 2022)

```bash
# Prerequisites: Visual Studio 2022, Python 3.10, Git
# Open "x64 Native Tools Command Prompt for VS 2022"

# Clone and prepare dependencies
git clone --recursive https://github.com/AyuGram/AyuGramDesktop.git
cd AyuGramDesktop/Telegram
build\prepare\win.bat

# Configure
configure.bat x64 -D TDESKTOP_API_ID=2040 -D TDESKTOP_API_HASH=b18441a1ff607e10a989891a5462e627

# Build via Visual Studio
# Open out\Telegram.sln in Visual Studio 2022
# Build > Build Telegram (Debug or Release)

# Or build via command line
cmake --build out --config Release
```

### macOS

```bash
# Prepare
./Telegram/build/prepare/mac.sh

# Configure
cd Telegram
./configure.sh -D TDESKTOP_API_ID=2040 -D TDESKTOP_API_HASH=b18441a1ff607e10a989891a5462e627

# Build
cmake --build ../out --config Release
```

### Linux

```bash
# Prepare dependencies
./Telegram/build/prepare/linux.sh

# Configure
cd Telegram
./configure.sh -D TDESKTOP_API_ID=2040 -D TDESKTOP_API_HASH=b18441a1ff607e10a989891a5462e627

# Build
cmake --build ../out --config Release
```

### Running Tests

```bash
cmake -DDESKTOP_APP_TEST_APPS=ON <build-dir>
cmake --build <build-dir> --target tests
```

## Architecture Overview

### Repository Structure

```
AGDesktop/
├── Telegram/                    # Main application
│   ├── SourceFiles/            # All C++ source code
│   │   ├── ayu/               # AyuGram-specific features (130+ files)
│   │   │   ├── data/          # Database & persistence layer
│   │   │   ├── features/      # Feature modules (filters, translator, streamer mode, etc.)
│   │   │   ├── ui/            # AyuGram UI components and settings pages
│   │   │   ├── utils/         # Helper utilities
│   │   │   ├── libs/          # Third-party libs (nlohmann/json, sqlite_orm)
│   │   │   ├── ayu_settings.h/cpp  # Central settings management
│   │   │   ├── ayu_state.h/cpp     # Global state container
│   │   │   └── ayu_infra.h/cpp     # Initialization coordinator
│   │   ├── core/              # Application lifecycle
│   │   ├── ui/                # Telegram UI framework
│   │   ├── data/              # Data models (User, Chat, Message)
│   │   ├── api/               # Telegram API wrappers
│   │   └── mtproto/           # Protocol layer
│   ├── lib_*/                 # Modular libraries (rpl, ui, base, tl, etc.)
│   ├── Resources/             # Assets, translations, icons
│   └── ThirdParty/            # Git submodules
├── cmake/                      # Build configuration helpers
└── docs/                       # Platform-specific build guides
```

### Relationship to Telegram Desktop

AyuGram Desktop is a fork that:
- **Inherits**: Core protocol, UI framework, data models, and networking from Telegram Desktop
- **Extends**: Adds 130+ settings and custom features in the `ayu/` directory
- **Maintains**: Compatibility with official Telegram protocol and regular upstream merges

All AyuGram-specific code is isolated in `Telegram/SourceFiles/ayu/`, making upstream merges cleaner.

### Core Libraries

| Library | Purpose | Notes |
|---------|---------|-------|
| `lib_rpl` | Reactive Programming Library - observable/producer pattern | Central to state management |
| `lib_crl` | Coroutines & async execution | Threading utilities |
| `lib_base` | Collections, utilities, type traits | Foundation |
| `lib_ui` | Qt widgets, animations, styling system | **AyuGram fork** with extensions |
| `lib_tl` | TL schema code generation for Telegram protocol | **AyuGram fork** |
| `lib_icu` | International Unicode support for regex | **AyuGram custom addition** |
| `lib_storage` | Encrypted local data persistence | Telegram Desktop |
| `lib_webrtc` | Voice/video call infrastructure | Telegram Desktop |

## Key AyuGram Features & Implementation

### Settings System (`ayu/ayu_settings.h`)

All 130+ AyuGram settings are defined in a single `AyuGramSettings` struct with reactive variables for UI binding.

**Key patterns:**
```cpp
// Reading settings
auto& settings = AyuSettings::getInstance();
if (settings.sendReadMessages) { /* ... */ }

// Writing settings (updates both value and reactive variable)
AyuSettings::set_sendReadMessages(false);
AyuSettings::save();  // Persist to tdata/ayu_settings.json

// Reactive binding in UI
rpl::single(settings.crashReporting) | rpl::start_with_next([=](bool enabled) {
    // React to changes
}, lifetime);
```

**Persistence**: Settings are stored in `tdata/ayu_settings.json` using nlohmann/json serialization with backward compatibility.

### Ghost Mode (Privacy)

**Location**: `ayu/ayu_settings.h/cpp`

Five boolean flags control privacy:
- `sendReadMessages` - Hide message read receipts
- `sendReadStories` - Hide story views
- `sendOnlinePackets` - Hide online status
- `sendUploadProgress` - Hide upload indicators
- `sendOfflinePacketAfterOnline` - Force offline packet after going online

These settings are checked throughout the codebase before sending protocol packets.

### Message History (Anti-Recall)

**Location**: `ayu/data/`

Stores all deleted and edited messages in SQLite database (`tdata/ayudata.db`).

**Key classes:**
- `DeletedMessage` - Full snapshot before deletion
- `EditedMessage` - All previous versions of edited messages
- `MessagesStorage` - Database access layer

**Database schema** uses sqlite_orm with indexes on `(userId, dialogId, topicId, messageId)`.

### Message Filters

**Location**: `ayu/features/filters/`

**Architecture:**
```
filters_controller.h       # Public API: isEnabled(), isBlocked(), filtered()
    ↓
filters_cache_controller.h # LRU cache of compiled regex patterns
    ↓
filters_utils.cpp          # ICU regex compilation & matching
```

**Key features:**
- Full Unicode regex support via lib_icu
- Per-dialog and global filters
- Case sensitivity toggle
- Reversed logic (show instead of hide)
- Shadow ban detection

**Performance**: Compiled patterns are cached to avoid recompilation on every message.

### Translator

**Location**: `ayu/features/translator/`

Inline message translation with multiple backends (Google, Telegram, Yandex).

**Architecture:**
- `TranslateManager` - Builder pattern API with LRU cache (max 500 entries)
- `implementations/` - Provider adapters
- Cache key: `{text_hash, fromLang, toLang}` or `{peerId, msgId, toLang}`

### Streamer Mode

**Location**: `ayu/features/streamer_mode/`

Platform-specific implementations to hide PII during screen sharing:
- `platform/streamer_mode_win.h/cpp` - Windows implementation
- `platform/streamer_mode_mac.h/cpp` - macOS implementation
- `platform/streamer_mode_linux.h/cpp` - Linux implementation

## Coding Patterns & Conventions

### Reactive Programming (RPL)

The codebase extensively uses the RPL library for state management. Key types:

```cpp
rpl::variable<T>      // Mutable observable state
rpl::producer<T>      // Read-only observable stream
rpl::event_stream<T>  // Event broadcaster
rpl::lifetime         // Subscription lifetime management

// Example usage
auto intProducer = rpl::single(123);
std::move(intProducer) | rpl::start_with_next([=](int value) {
    // Handle value
}, lifetime);

// Transformations
std::move(producer)
    | rpl::map([](int x) { return x * 2; })
    | rpl::filter([](int x) { return x > 10; })
    | rpl::start_with_next([=](int value) { /* ... */ }, lifetime);

// Combining producers (lambdas receive unpacked arguments)
rpl::combine(producer1, producer2) | rpl::start_with_next([=](Type1 val1, Type2 val2) {
    // Handle combined values
}, lifetime);
```

**Important**: Always use `std::move()` when starting a producer chain, and store the returned `rpl::lifetime` if not passing one as an argument.

### UI Styling System

Styles are defined in `.style` files using a custom syntax and code-generated into C++ objects in the `st::` namespace.

**Example** (`ayu/ui/ayu_styles.style`):
```style
using "ui/basic.style";

AyuButtonStyle {
    textPadding: margins;
    icon: icon;
    height: pixels;
}

ayuPrimaryButton: AyuButtonStyle {
    textPadding: margins(10px, 15px, 10px, 15px);
    icon: icon{{ "gui/icons/check", iconColor }};
    height: 40px;
}
```

**Usage in C++:**
```cpp
#include "styles/style_ayu.h"

int height = st::ayuPrimaryButton.height;
const style::icon &icon = st::ayuPrimaryButton.icon;
myButton->setIcon(icon);
```

### Localization

Strings are defined in `Telegram/Resources/langs/lang.strings` and accessed via the `tr::` namespace.

```cpp
// Immediate value
auto title = tr::lng_settings_title(tr::now);  // Returns QString

// Reactive producer
auto titleProducer = tr::lng_settings_title();  // Returns rpl::producer<QString>

// With placeholders
auto text = tr::lng_confirm_delete_item(tr::now, lt_item_name, itemName);

// With pluralization
auto filesText = tr::lng_files_selected(tr::now, lt_count, count);

// Reactive with placeholders (move producers)
auto textProducer = tr::lng_confirm_delete_item(
    lt_item_name,
    std::move(itemNameProducer)
);
```

### Telegram API Usage

API methods are defined in `Telegram/SourceFiles/mtproto/scheme/api.tl` and code-generated into `MTP*` types.

```cpp
api().request(MTPmessages_SendMessage(
    MTP_flags(flags_value),
    MTP_inputPeer(peer),
    MTP_string(messageText),
    MTP_long(randomId),
    MTP_vector<MTPMessageEntity>()
)).done([=](const MTPUpdates &result) {
    // Handle response with .match() for multiple constructors
    result.match([&](const MTPDupdates &data) {
        // Use data.vupdates().v
    }, [&](const MTPDupdateShort &data) {
        // Handle short update
    });
}).fail([=](const MTP::Error &error) {
    // Handle error
    if (error.type() == u"FLOOD_WAIT_X"_q) {
        // Handle flood wait
    }
}).handleFloodErrors().send();
```

### Database Access (sqlite_orm)

```cpp
// Define schema
auto storage = make_storage(
    "./tdata/ayudata.db",
    make_table<EditedMessage>("EditedMessage",
        make_column("userId", &EditedMessage::userId),
        make_column("messageId", &EditedMessage::messageId),
        // ...
    ),
    make_index("idx_userId_messageId", ...)
);

// Type-safe queries
auto messages = storage.select(
    &EditedMessage::text,
    where(c(&EditedMessage::userId) == user_id),
    order_by(&EditedMessage::messageId).desc(),
    limit(100)
);
```

## Development Workflow

### Adding New Features

1. **Define Settings** - Add to `ayu/ayu_settings.h` struct and JSON serialization
2. **Create Feature Module** - Add to `ayu/features/{feature}/` with clear controller/implementation separation
3. **Build UI** - Create settings page in `ayu/ui/settings/settings_{feature}.h/cpp`
4. **Initialize** - Hook into `AyuInfra::init()` or `AyuWorker::initialize()`
5. **Test** - Build on all platforms (Win/Mac/Linux) and verify functionality
6. **Document** - Update relevant documentation

### Code Style

- Use `auto` for type deduction (very common in this codebase)
- Prefer `const auto &` for references
- Always `std::move()` producers when starting chains
- Pass `rpl::lifetime` to manage subscription lifetimes
- Use reactive variables for UI state binding
- Include generated style headers: `#include "styles/style_*.h"`

### Common Patterns

**Settings Toggle in UI:**
```cpp
AddButtonWithIcon(
    container,
    tr::ayu_FeatureName(),
    st::settingsButton,
    {&st::menuIconFeature}
)->toggleOn(
    rpl::single(settings->featureEnabled)
)->toggledValue(
) | rpl::filter([=](bool enabled) {
    return (enabled != settings->featureEnabled);
}) | start_with_next([=](bool enabled) {
    AyuSettings::set_featureEnabled(enabled);
    AyuSettings::save();
}, container->lifetime());
```

**Feature Controller Pattern:**
```cpp
class FeatureController {
public:
    static FeatureController& getInstance();

    bool isEnabled() const;
    void process(const Data &data);

private:
    FeatureController();
    // Implementation details
};
```

## Important Notes

- **Crash Reporting**: As of latest changes, crash reporting is now **disabled by default** (`crashReporting = false` in `ayu_settings.cpp:337`). Users must opt-in via Settings > Other > Crash Reporting.
- **Platform Differences**: Some features have platform-specific implementations (see `ayu/features/streamer_mode/platform/`)
- **Upstream Merges**: Regularly rebase with Telegram Desktop's dev branch. Keep `ayu/` changes isolated.
- **API Credentials**: Use test credentials for development (see `docs/api_credentials.md`)

## Debugging

```bash
# Enable Qt logging
export QT_LOGGING_RULES="qt.qpa.*=true"

# Run with debug output
./Telegram --debug

# Check settings file
cat ~/.local/share/TelegramDesktop/tdata/ayu_settings.json

# Inspect database
sqlite3 ~/.local/share/TelegramDesktop/tdata/ayudata.db
```

## Resources

- **Documentation**: https://docs.ayugram.one/desktop/
- **Repository**: https://github.com/AyuGram/AyuGramDesktop
- **Upstream**: https://github.com/telegramdesktop/tdesktop
- **Qt Documentation**: https://doc.qt.io/qt-6/
- **Cursor Rules**: See `.cursor/rules/` for detailed guides on styling, RPL, API usage, and localization
