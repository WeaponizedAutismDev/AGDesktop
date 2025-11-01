# ATH0Gram Rebrand - Detailed Implementation Plan

**Branch**: `feature/ath0gram-rebrand`
**Last Updated**: 2025-11-02

---

## Implementation Strategy

**Approach**: Small, atomic commits organized by functional area
**Testing**: Build test after each major section
**Rollback**: Git tags at each phase completion

---

## Phase 2: Text & String Rebrand

### ⚠️ BUILD SAFETY NOTES

**CRITICAL**: These phases must be done atomically to avoid build breaks:

1. **Phase 2.2 + 2.3 = ATOMIC**
   - Changing `lang.strings` keys breaks C++ code that references old keys
   - MUST update both in same commit, single build test after

2. **Icon Paths = KEEP AS "ayu/" FOR NOW**
   - Style files reference `icon {{ "ayu/ghost", color }}`
   - If we rename directory but miss updating a .style file → build breaks
   - If .style references icon that doesn't exist → build breaks
   - **SOLUTION**: Keep `Resources/icons/ayu/` path unchanged in Phase 2
   - Rename in Phase 3 only when replacement icons ready

**See ICON_INVENTORY.md for full icon strategy**

---

### Phase 2.1: Core Infrastructure 🔧

**Goal**: Update foundational classes and file paths (NON-BREAKING changes only)

#### Files to Modify:

1. **`Telegram/SourceFiles/ayu/ayu_settings.cpp`** (Line 337)
   - ✅ Already changed `crashReporting = false` (committed)
   - Change JSON file path:
     ```cpp
     // OLD: "ayu_settings.json"
     // NEW: "ath0_settings.json"
     ```

2. **`Telegram/SourceFiles/ayu/data/ayu_database.cpp`**
   - Database filename:
     ```cpp
     // OLD: "ayudata.db"
     // NEW: "ath0data.db"
     ```

3. **`Telegram/SourceFiles/core/version.h`**
   - App display name references
   - Version identifier string

4. **Namespace/Class Comments** (all `ayu/*.h` files)
   - Update copyright headers: Add "Modified by WeaponizedAutism, 2025"
   - Keep original "Copyright @Radolyn, 2025" attribution
   - Update "AyuGram for Desktop" → "ATH0Gram for Desktop"

**Estimated Files**: ~15-20
**Test**: Settings save/load, database access

---

### Phase 2.2 + 2.3: Language Strings + UI Code (ATOMIC) 📝🎨

**⚠️ MUST BE DONE TOGETHER** - Single commit, single build test

**Goal**: Update language keys AND C++ code that references them

#### Part A: Language Strings File

**`Telegram/Resources/langs/lang.strings`**

**Search & Replace Strategy**:

| Old Pattern | New Pattern | Count (est.) |
|-------------|-------------|--------------|
| `AyuGram` | `ATH0Gram` | ~50 |
| `Ayugram` | `ATH0gram` | ~10 |
| `ayu_` (prefix) | `ath0_` (prefix) | ~200+ keys |
| Ghost Mode | Disconnected Mode | ~15 |
| Ghost feature | No Carrier / Disconnected | ~10 |
| Streamer Mode | Blackout Mode | ~8 |
| Streamer feature | Blackout | ~5 |

**Special Replacements** (context-sensitive):

```
// Taglines & Descriptions
"Privacy-focused Telegram client" → "Disconnected from the noise"
"Enhanced Telegram experience" → "ATH0: No carrier detected"
"Made with love" → "Weaponized for privacy"

// Ghost Mode specific
lng_ayu_ghost_mode → lng_ath0_disconnected_mode
lng_ayu_ghost_description → lng_ath0_disconnected_description
lng_ayu_enable_ghost → lng_ath0_enable_disconnected
"hide your activity" → "stay disconnected"
"invisible mode" → "no carrier mode"

// Streamer Mode specific
lng_ayu_streamer_mode → lng_ath0_blackout_mode
lng_ayu_streamer_description → lng_ath0_blackout_description
"protect your screen" → "blackout mode engaged"
```

#### Part B: UI Component Code (SAME COMMIT!)

**Goal**: Update C++ code referencing the renamed lang keys from Part A

#### Files by Category (update tr::ayu_* → tr::ath0_*):

**Settings Pages** (`ayu/ui/settings/`)
- ✅ `settings_main.cpp` - Main AyuGram settings entry
- ✅ `settings_appearance.cpp`
- ✅ `settings_general.cpp` - Ghost/Disconnected mode
- ✅ `settings_chats.cpp`
- ✅ `settings_other.cpp` - Crash reporting, streamer/blackout
- ✅ `settings_filters.cpp`

**Dialog Boxes** (`ayu/ui/boxes/`)
- ✅ `donate_info_box.cpp` - Supporter badge text (will be removed)
- ✅ `donate_qr_box.cpp` - Same
- ✅ `import_filters_box.cpp`
- ✅ `message_shot_box.cpp`
- ✅ `theme_selector_box.cpp`

**Components** (`ayu/ui/components/`)
- ✅ `icon_picker.cpp`
- ✅ `saved_music.cpp`

**Message History** (`ayu/ui/message_history/`)
- ✅ All 3 files (history_inner, history_item, history_section)

**Changes**: Update `tr::ayu_*` → `tr::ath0_*` language key references

**Estimated Files (Parts A + B combined)**: ~26
**Commit Message**: `rebrand: update language strings and UI components (ayu→ath0, ghost→disconnected)`
**Test**: ✅ **BUILD TEST** - Launch app, navigate all settings, check all dialogs

---

### Phase 2.4: Platform-Specific Code 💻

**Goal**: Update executable names, app identifiers, desktop files

#### Windows (`platform/win/`)

1. **`windows_app_user_model_id.cpp`**
   ```cpp
   // Lines 29-31
   // OLD: L"AyuGram.AyuGramDesktop.Store"
   // NEW: L"ATH0Gram.ATH0GramDesktop.Store"

   // OLD: L"AyuGram.AyuGramDesktop"
   // NEW: L"ATH0Gram.ATH0GramDesktop"

   // OLD: u"AyuGram.lnk"
   // NEW: u"ATH0Gram.lnk"

   // OLD: u"AyuGram Desktop/AyuGram.lnk"
   // NEW: u"ATH0Gram Desktop/ATH0Gram.lnk"
   ```

2. **`specific_win.cpp`**
   ```cpp
   // Lines 323, 326
   // OLD: "\\AyuGram.lnk"
   // NEW: "\\ATH0Gram.lnk"
   ```

3. **`tray_win.cpp`** - Check for app name references

4. **`main_window_win.cpp`** - Check window title

#### macOS (`platform/mac/`)

1. **`specific_mac_p.mm`** - Bundle identifier
   ```objc
   // Check for com.ayugram.desktop
   // Replace with com.ath0gram.desktop
   ```

2. **`main_window_mac.mm`** - Window title, app name
3. **`window_title_mac.mm`** - Title bar

#### Linux (`platform/linux/`)

1. **`specific_linux.cpp`**
   ```cpp
   // Line 237
   // OLD: ":/misc/com.ayugram.desktop.desktop"
   // NEW: ":/misc/com.ath0gram.desktop.desktop"

   // Lines 376, 385, 692, 696
   // OLD: "ayugram.desktop"
   // NEW: "ath0gram.desktop"

   // OLD: "com.ayugram.desktop"
   // NEW: "com.ath0gram.desktop"
   ```

2. **`main_window_linux.cpp`** - Window class

#### Updater Files (ALL PLATFORMS) 🚨 CRITICAL

1. **`_other/updater_win.cpp`**
   ```cpp
   // Lines 207, 380, 387
   // OLD: L"AyuGram.exe"
   // NEW: L"ATH0Gram.exe"
   ```

2. **`_other/updater_osx.m`**
   ```objc
   // Line 11
   // OLD: @"AyuGram.app"
   // NEW: @"ATH0Gram.app"
   ```

3. **`_other/startup_task_win.cpp`**
   ```cpp
   // Line 49
   // OLD: "\\AyuGram.exe"
   // NEW: "\\ATH0Gram.exe"
   ```

**IMPORTANT**: Executable name MUST match CMake output!

**Estimated Files**: ~12
**Test**: Build on each platform, test shortcuts, test window titles

---

### Phase 2.5: External Links & Domains 🌐

**Goal**: Replace all ayugram.* and radolyn.* domains

#### Domain Mapping:

| Old Domain | New Domain | Purpose |
|------------|------------|---------|
| `ayugram.one` | `weaponizedautism.dev` | Main website |
| `update.ayugram.one` | *(disabled)* | Updates (disabled) |
| `translate.ayugram.one` | *(remove link)* | Translation portal |
| `docs.ayugram.one` | `weaponizedautism.dev/docs` | Documentation |
| `sentry.radolyn.com` | *(disabled)* | Crash reports (opt-in only) |
| `cdn.jsdelivr.net/gh/AyuGram/` | *(disabled)* | Language CDN |

#### Files to Modify:

1. **`window/window_main_menu.cpp`** (Line 385)
   ```cpp
   // OLD: u"https://ayugram.one"_q
   // NEW: u"https://weaponizedautism.dev"_q
   ```

2. **`ayu/ui/settings/settings_main.cpp`** (Lines 153, 158, 162)
   ```cpp
   // OLD: "https://translate.ayugram.one"
   // ACTION: Remove "Translate" button entirely

   // OLD: "docs.ayugram.one"
   // NEW: "docs.weaponizedautism.dev"
   ```

3. **`storage/localstorage.cpp`** (Lines 562, 571)
   ```cpp
   // OLD: "https://update.ayugram.one/"
   // NEW: Return empty string (disable auto-updates)
   ```

4. **`ayu/utils/rc_manager.cpp`** (Lines 54, entire class)
   ```cpp
   // OPTION 1: Disable in init
   void RCManager::start() {
       // Comment out or early return
       return; // Disabled for ATH0Gram
   }

   // OPTION 2: Remove network call
   // Keep data structures but use only default empty sets
   ```

5. **`ayu/ayu_lang.cpp`** (Lines 50, 53)
   ```cpp
   // Disable language updates - comment out network request
   // or return early in function
   ```

6. **`ayu/utils/telegram_helpers.cpp`** (Line 868)
   - Comment reference to GitHub PR (informational only)

7. **`ayu/utils/windows_utils.cpp`** (Lines 52, 142, 156)
   ```cpp
   // OLD: "/AyuGram.lnk", "/AyuGram.ico"
   // NEW: "/ATH0Gram.lnk", "/ATH0Gram.ico"
   ```

8. **`core/crash_report_window.cpp`** (Line 611)
   ```cpp
   // OLD: u"https://sentry.radolyn.com/..."
   // NEW: u"https://crashreport.weaponizedautism.dev/..."
   // OR: Return early / disable (crashes already opt-in)
   ```

**Estimated Files**: ~10
**Test**: Click all help/link buttons, verify no network calls to old domains

---

### Phase 2.6: Feature-Specific Renames 🎭

**Goal**: Update Ghost → Disconnected, Streamer → Blackout in code

#### Feature: Ghost Mode → Disconnected Mode

**Files** (~50+ references across codebase)

**Strategy**: These are mostly in language keys (already done in Phase 2.2) and comments

**Code changes**:
- Variable names: `ghostMode` → `disconnectedMode` (optional, not user-facing)
- Function names: `isGhostEnabled()` → `isDisconnectedEnabled()` (optional)
- Comments: Update for clarity

**Priority**: Low (no functional impact if just renaming user-facing text)

#### Feature: Streamer Mode → Blackout Mode

**Files in `ayu/features/streamer_mode/`**:
- `streamer_mode.h/cpp`
- `platform/streamer_mode_*.{h,cpp}` (3 platforms)

**Options**:
1. **Keep file/class names** (internal) but update UI strings (done in 2.2)
2. **Rename files/classes**: `streamer_mode` → `blackout_mode` ⚠️ RISKY for build

**Recommendation**: Option 1 (internal names don't matter)

**Estimated Files**: 7
**Test**: Enable/disable blackout mode, test on all platforms

---

### Phase 2.7: Supporter Badge Removal 🗑️

**Goal**: Completely disable RCManager and badge features

#### Strategy: Disable, Don't Delete

**Reason**: Less invasive, easier to undo, preserves code structure for upstream merges

#### Files to Modify:

1. **`ayu/utils/rc_manager.cpp`**
   ```cpp
   void RCManager::start() {
       LOG(("RCManager: disabled for ATH0Gram"));
       return; // Early return - don't start timer or network
   }

   // Lines 15-32: Clear default IDs
   std::unordered_set<ID> default_developers = {};
   std::unordered_set<ID> default_channels = {};
   ```

2. **`ayu/utils/telegram_helpers.cpp`**
   ```cpp
   // Lines 105-110
   bool isSupporterPeer(ID peerId) {
       return false; // Disabled
   }

   bool isCustomBadgePeer(ID peerId) {
       return false; // Disabled
   }

   // Line 122: ExteraBadgeTypeFromPeer
   // Always return Badge::Content{BadgeType::None}
   ```

3. **`ayu/ui/boxes/donate_*.cpp`** (2 files)
   - Keep files but disable entry points
   - OR: Comment out menu items that open these boxes

4. **`ayu/ui/ayu_icons.style`** (Lines 34-35, 52-59)
   - Keep style definitions (harmless) or comment out

**Estimated Files**: 5
**Test**: Check profiles - no badges should appear

---

## Phase 3: Icons & Logos 🎨

**⚠️ OPTIONAL PHASE** - Can be done much later or never

**Current Status**: All existing AyuGram icons work fine, just keep using them
**Only do Phase 3 if**: You want custom ATH0Gram icon designs

**Strategy**: Keep `Resources/icons/ayu/` and `Resources/art/ayu/` paths for now
- Users never see these internal paths
- Style files reference `"ayu/"` path (internal)
- Only visible branding (text, names) is rebranded
- Icons can be replaced later without rebuild

---

### Phase 3.1: Icon Preparation (YOUR ACTION REQUIRED - IF DOING THIS PHASE)

**What You Need to Provide**:

12 `.ico` files matching these specs:
- **Format**: Windows ICO (multi-resolution: 16x16, 32x32, 48x48, 256x256)
- **Color Depth**: 32-bit RGBA
- **Size Range**: 18KB - 60KB typical
- **Themes to Replace**:

| Theme | Path | Current Size | Description |
|-------|------|--------------|-------------|
| **default** ⭐ | `ath0/default/app_icon.ico` | 30 KB | Primary - REQUIRED |
| alt | `ath0/alt/app_icon.ico` | 25 KB | Alternative design |
| discord | `ath0/discord/app_icon.ico` | 28 KB | Discord-style |
| nothing | `ath0/nothing/app_icon.ico` | 27 KB | Nothing Phone style |
| spotify | `ath0/spotify/app_icon.ico` | 27 KB | Spotify-style |
| win95 | `ath0/win95/app_icon.ico` | 35 KB | Retro Windows 95 |
| *(your choice)* | More themes as desired | ~30 KB | Custom designs |

**DELETE These** (Ayugram partner branding):
- ❌ `ayu/extera/` folder
- ❌ `ayu/extera2/` folder
- ❌ `ayu/bard/` (?) - your choice
- ❌ `ayu/chibi/` and `ayu/chibi2/` (?) - your choice
- ❌ `ayu/yaplus/` (Yandex branding)

**Menu Icons** (PNG):
- `icons/ath0/ath0_menu.png` (48x48)
- `icons/ath0/ath0_menu@2x.png` (96x96)
- `icons/ath0/ath0_menu@3x.png` (144x144)

**Tool**: Use IcoFX, GIMP, or ImageMagick to create multi-resolution .ico files

---

### Phase 3.2: Icon Installation

#### Files to Modify:

1. **Rename Directory**:
   ```bash
   mv Telegram/Resources/art/ayu Telegram/Resources/art/ath0
   mv Telegram/Resources/icons/ayu Telegram/Resources/icons/ath0
   ```

2. **Update Icon Picker** - `ayu/ui/components/icon_picker.cpp`
   - Change path references: `"art/ayu/"` → `"art/ath0/"`
   - Update theme names in UI

3. **Update Logo Component** - `ayu/ui/ayu_logo.cpp`
   - Line 25: `"AyuGram.ico"` → `"ATH0Gram.ico"`
   - Path references to icon directory

4. **Update Style Files** - `ayu/ui/ayu_icons.style`
   - Icon path references: `"ayu/"` → `"ath0/"`

5. **Platform Resources** (build system reads these):
   - May need CMakeLists.txt changes ⚠️ (test carefully!)
   - OR: Keep `ayu/` as internal path, only change files

**Estimated Files**: 5-10
**Test**: Icon picker, tray icon, window icon, taskbar icon

---

### Phase 3.3: Badge Icon Cleanup

**Delete/Replace** (from `icons/ath0/`):
- ❌ `donates/support_logo.svg` (no more donations/supporters)

**Update References** (or comment out):
- `ayu/ui/ayu_icons.style` - Supporter badge icons
- Remove: `infoExteraOfficialBadge`, `infoExteraSupporterBadge`
- Remove: `dialogsExteraOfficialIcon`, `dialogsExteraSupporterIcon`

**Estimated Files**: 2
**Test**: No missing icon errors in logs

---

## Phase 4: Privacy & Security Cleanup 🔒

### Phase 4.1: Network Call Audit

**Goal**: Verify no unexpected outbound connections

#### Testing Method:

1. Build ATH0Gram with all changes
2. Run with network monitor (Wireshark, Charles Proxy, Fiddler)
3. Interact with all features:
   - Open settings
   - Change icon
   - View message history
   - Use translator (expected: Google/Yandex/Telegram APIs only)
   - Check for updates (should be disabled)
4. Verify ONLY Telegram API calls (`149.154.*`, official Telegram IPs)

#### Expected Connections:

✅ **ALLOWED**:
- `149.154.167.*` - Telegram API
- `149.154.175.*` - Telegram Media
- Google/Yandex/Telegram translate APIs (if using translator)

❌ **BLOCKED** (should NOT appear):
- `update.ayugram.one`
- `sentry.radolyn.com`
- `cdn.jsdelivr.net/gh/AyuGram/`
- Any other non-Telegram domains

---

### Phase 4.2: TelegramDB Bot Review

**Bot ID**: 7424190611 (`usernameResolverBotId`)

**Action**: Keep as-is (user decision)

**Reasoning**:
- ✅ Bellingcat OSINT toolkit bot (legitimate tool)
- ✅ Free tier: `/resolve` command for ID ↔ username lookups
- ✅ Not AyuGram-specific infrastructure
- ✅ Useful for research/investigation

**Documentation**: Add note to docs explaining bot usage:
```
TelegramDB Bot (7424190611): Free OSINT tool for username resolution
- Part of Bellingcat investigation toolkit
- No connection to AyuGram infrastructure
- Free tier: /resolve command for user/group/channel lookups
- More info: [bellingcat toolkit docs link]
```

---

### Phase 4.3: Data Privacy Verification

**Goal**: Ensure user data stays local

#### Files to Verify:

1. **`ayu/data/ayu_database.cpp`**
   - ✅ SQLite database is local only
   - ✅ No cloud sync or external DB connections
   - ✅ Encrypted with Telegram's storage layer

2. **`ayu/ayu_settings.cpp`**
   - ✅ Settings saved to local JSON
   - ✅ No telemetry in settings
   - ✅ Crash reporting: opt-in only (default: false)

3. **Message History Storage**
   - ✅ All deleted/edited messages stored locally
   - ✅ No external backup or sync

**Result**: ✅ All user data is local-only (as expected)

---

## Phase 5: Testing & Validation ✅

### Phase 5.1: Build Testing

**Platform**: Windows (primary)
**Toolchain**: Visual Studio 2022, x64 Native Tools Command Prompt

#### Build Steps:

1. Clean build:
   ```cmd
   cd out
   cmake --build . --config Debug --clean-first
   ```

2. Monitor for:
   - ❌ Missing language keys
   - ❌ Missing icon files
   - ❌ Linker errors from renamed classes
   - ❌ Resource compilation errors

3. Run executable:
   ```cmd
   out\Debug\ATH0Gram.exe --debug
   ```

4. Check log for errors

---

### Phase 5.2: Functional Testing

**Test Matrix**:

| Feature | Test Action | Expected Result | Status |
|---------|-------------|-----------------|--------|
| **Branding** | Open app | Title: "ATH0Gram" | ⏳ |
| | About dialog | ATH0Gram version info | ⏳ |
| | Tray icon | ATH0Gram icon | ⏳ |
| **Settings** | Open AyuGram settings | "ATH0Gram Settings" header | ⏳ |
| | Navigate all pages | No "AyuGram" visible | ⏳ |
| **Ghost Mode** | Enable disconnected mode | Works, labeled "Disconnected" | ⏳ |
| | Check tooltips | "No Carrier" / "Disconnected" terms | ⏳ |
| **Blackout** | Enable blackout mode | Works, labeled "Blackout Mode" | ⏳ |
| **Icons** | Open icon picker | Shows ATH0Gram themes | ⏳ |
| | Change icon | Updates tray/window icon | ⏳ |
| **Privacy** | Check network | No calls to ayugram domains | ⏳ |
| | Open supporter profiles | No badges shown | ⏳ |
| | Check for updates | Disabled / no action | ⏳ |
| **Links** | Help → Website | Opens weaponizedautism.dev | ⏳ |
| | Help → Docs | Opens weaponizedautism.dev/docs | ⏳ |
| **Data** | Save settings | Writes ath0_settings.json | ⏳ |
| | Message history | Uses ath0data.db | ⏳ |

---

### Phase 5.3: Upstream Merge Simulation

**Goal**: Verify rebrand survives upstream merges

#### Test Procedure:

1. Tag current rebrand: `git tag rebrand-v1.0`
2. Fetch latest upstream: `git fetch upstream`
3. Create test branch: `git checkout -b test-upstream-merge`
4. Attempt merge: `git merge upstream/dev`
5. Review conflicts:
   - Expected: `Resources/langs/lang.strings` (our ATH0 strings vs their new features)
   - Expected: Possibly `ayu/ui/settings/` if they add new pages
   - Unexpected: Build system conflicts (should not happen!)
6. Resolve conflicts, rebuild, test
7. If successful: Document merge strategy
8. Discard test branch: `git branch -D test-upstream-merge`

**Documentation**: Create `MERGING_UPSTREAM.md` guide

---

## Phase 6: Documentation & Finalization 📚

### Phase 6.1: Update Project Documentation

**Files to Create/Update**:

1. **`README.md`** (create if not exists)
   ```markdown
   # ATH0Gram Desktop

   Privacy-focused Telegram fork - Disconnected from the noise.

   Based on AyuGram Desktop by @Radolyn
   Modified by WeaponizedAutism for enhanced privacy.

   ## Features
   - Disconnected Mode (formerly Ghost)
   - Blackout Mode (formerly Streamer)
   - Message History (anti-recall)
   - Advanced filters
   - No telemetry, no phoning home
   - Manual updates only

   ## Building
   [Link to weaponizedautism.dev/docs]

   ## Attribution
   Original work: AyuGram Desktop (github.com/AyuGram/AyuGramDesktop)
   Upstream: Telegram Desktop (github.com/telegramdesktop/tdesktop)
   ```

2. **`CLAUDE.md`** (update existing)
   - Change all "AyuGram" → "ATH0Gram"
   - Update repository URLs
   - Add privacy stance section
   - Update feature names (Disconnected, Blackout)

3. **`LICENSE`** (verify/update)
   - Ensure GPL-3.0 compliance
   - Add attribution to AyuGram
   - Add your copyright for modifications

4. **`.github/` or docs folder** (optional)
   - `PRIVACY.md` - Document privacy features
   - `MERGING_UPSTREAM.md` - Upstream merge guide
   - `ICON_GUIDE.md` - How to add custom icons

---

### Phase 6.2: Commit Organization

**Git Strategy**:

#### Commit Structure:

```
rebrand: core infrastructure and settings paths
rebrand: update all language strings (ayu→ath0, ghost→disconnected)
rebrand: ui components and settings pages
rebrand: platform-specific identifiers and exe names
rebrand: replace external domain references
rebrand: disable RCManager and supporter badges
rebrand: feature rename (ghost→disconnected, streamer→blackout)
rebrand: update project documentation
icons: replace ayugram icons with ath0gram themes
privacy: disable auto-updates and language CDN
privacy: document TelegramDB bot usage
docs: update README and CLAUDE.md for ath0gram
chore: final cleanup and attribution
```

#### Tags:

- `rebrand-phase2-complete` - After all text/code changes
- `rebrand-phase3-complete` - After icon replacement
- `rebrand-phase4-complete` - After privacy audit
- `rebrand-complete` - Final release-ready state

---

### Phase 6.3: Merge to Dev

**Final Steps**:

1. **Squash or Keep History?**
   - **Option A**: Keep detailed commits (recommended) - easier to review/debug
   - **Option B**: Squash to single commit - cleaner but loses granularity

2. **Pre-Merge Checklist**:
   - ✅ All tests passing
   - ✅ Build successful on Windows (and Mac/Linux if available)
   - ✅ No network calls to old domains
   - ✅ Documentation updated
   - ✅ Attribution preserved

3. **Merge**:
   ```bash
   git checkout dev
   git merge --no-ff feature/ath0gram-rebrand
   git tag rebrand-v1.0.0
   git push origin dev --tags
   ```

4. **Cleanup**:
   ```bash
   git branch -d feature/ath0gram-rebrand
   ```

---

## Next Steps for YOU 👤

### Immediate Actions:

1. **Icon Preparation** 🎨
   - [ ] Design ATH0Gram logo/icons
   - [ ] Create 12 `.ico` files (or use fewer themes)
   - [ ] Create menu PNGs (@1x, @2x, @3x)
   - [ ] Verify format: multi-res ICO (16, 32, 48, 256)

2. **Domain Setup** 🌐
   - [ ] Point `weaponizedautism.dev` to your site
   - [ ] (Optional) Create `/docs` page
   - [ ] (Optional) Set up crash report endpoint

3. **Review Recon Report** 📋
   - [ ] Read `RECON_NOTES.md` fully
   - [ ] Verify no concerns with findings
   - [ ] Approve implementation plan

### When Ready to Start:

**Let me know and I'll begin Phase 2.1** (Core Infrastructure)

We'll work through each phase systematically, with builds/tests between phases.

---

## Estimated Timeline ⏱️

**Assumptions**:
- You're available for decisions
- Icons are ready
- Building on Windows only initially

| Phase | Description | Estimated Time | Build Test Required |
|-------|-------------|----------------|---------------------|
| 2.1 | Core infrastructure | 1-2 hours | ✅ Yes |
| 2.2 | Language strings | 2-3 hours | ✅ Yes (thorough UI check) |
| 2.3 | UI components | 2-3 hours | ✅ Yes |
| 2.4 | Platform code | 2-3 hours | ✅ Yes (critical!) |
| 2.5 | External links | 1 hour | ⏭️ Quick check |
| 2.6 | Feature renames | 1 hour | ⏭️ Quick check |
| 2.7 | Badge removal | 1 hour | ✅ Yes |
| 3.1-3.3 | Icons | *Waiting on assets* + 2 hours | ✅ Yes |
| 4.1-4.3 | Privacy audit | 2 hours | ✅ Yes (network monitor) |
| 5.1-5.3 | Testing | 3-4 hours | ✅ Comprehensive |
| 6.1-6.3 | Documentation | 2 hours | ⏭️ No build |

**Total**: ~20-25 hours of work (can be spread over days)

**Build Time Per Test**: ~5-15 min (incremental), ~30-60 min (clean)

---

## Emergency Rollback Plan 🔴

If something breaks catastrophically:

```bash
# Rollback to last known good state
git checkout dev
git reset --hard rebrand-phase2-complete  # or earlier tag

# Or create recovery branch
git checkout -b recovery-[phase]
git revert [bad-commit-hash]
```

**Prevention**:
- Commit frequently
- Tag after each phase
- Test builds regularly
- Don't touch build system files!

---

**Implementation Plan Complete** ✅

Ready to proceed when you are!
