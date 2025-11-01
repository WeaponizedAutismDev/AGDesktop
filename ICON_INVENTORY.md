# ATH0Gram Icon Inventory

**Critical**: These files MUST exist for builds to succeed!

---

## PNG UI Icons (Referenced in .style files)

**Location**: `Telegram/Resources/icons/ayu/`

All icons need 3 resolutions: `name.png`, `name@2x.png`, `name@3x.png`

### Feature Icons:
- ✅ **ghost.png** (3x) - Disconnected/Ghost mode icon
- ✅ **ghost_tray.png** (3x) - Tray icon for disconnected mode
- ✅ **streamer.png** (3x) - Blackout/Streamer mode icon
- ✅ **ayu_menu.png** (3x) - Main menu icon

### History Icons:
- ✅ **edits_history.png** (3x) - Message edit history
- ✅ **edited.png** (3x) - Edited message indicator
- ✅ **trash_bin.png** (3x) - Deleted message indicator

### Read Status Icons:
- ✅ **lread.png** (3x) - Large read indicator
- ✅ **sread.png** (3x) - Small read indicator

### Message Field Icons:
- ✅ **message_field/attach.png** (3x)
- ✅ **message_field/commands.png** (3x)
- ✅ **message_field/emoji.png** (3x)
- ✅ **message_field/voice.png** (3x)
- ✅ **message_field/ttl.png** (3x)

### Channel Badge:
- ✅ **channel.png** (3x)

### Navigation:
- ✅ **to_beginning.png** (3x)

### Supporter Badges (TO BE REMOVED):
- ❌ **extera_official.png** (3x) - Delete after disabling
- ❌ **extera_badge.png** (3x) - Delete after disabling
- ❌ **dialogs_extera_official.png** (3x) - Delete after disabling
- ❌ **dialogs_extera_supporter.png** (3x) - Delete after disabling

**Total**: 17 icons × 3 resolutions = **51 PNG files** (excluding 12 extera files to delete)

---

## App Icons (Windows .ico)

**Location**: `Telegram/Resources/art/ayu/[theme]/app_icon.ico`

### Existing Themes:
1. ✅ **default/** - Primary icon (30KB)
2. ✅ **alt/** - Alternative
3. ✅ **bard/** - Bard theme
4. ✅ **chibi/** - Chibi character
5. ✅ **chibi2/** - Chibi variant
6. ✅ **discord/** - Discord-style
7. ❌ **extera/** - Delete (partner branding)
8. ❌ **extera2/** - Delete (partner branding)
9. ✅ **nothing/** - Nothing Phone style
10. ✅ **spotify/** - Spotify-style
11. ✅ **win95/** - Windows 95 retro
12. ✅ **yaplus/** - Yandex Plus style

**Keep**: 10 themes, **Delete**: 2 extera themes

---

## SVG Files

### App Icon Sources
**Location**: `Telegram/Resources/art/ayu/[theme]/app.svg`

These are source SVGs (vector) that get compiled to .ico files. Same themes as above.

### Donation Icons (TO BE DELETED)
**Location**: `Telegram/Resources/icons/ayu/donates/`

- ❌ bitcoin.svg
- ❌ ethereum.svg
- ❌ solana.svg
- ❌ ton.svg
- ❌ tron.svg
- ❌ boosty.svg
- ❌ support_logo.svg

All related to donation/supporter features - safe to delete.

### Other SVGs:
- ✅ **nocover.svg** - Placeholder for missing album art

---

## Build Safety Strategy

### ⚠️ CRITICAL: Icon Paths in Style Files

The `.style` code generator runs at compile time. If it references:
```style
icon {{ "ayu/ghost", color }}
```

Then `Resources/icons/ayu/ghost.png` (and @2x, @3x) **MUST exist** or build fails!

### Safe Implementation Options:

#### **Option 1: Keep "ayu/" path initially** ✅ RECOMMENDED
- Don't rename `icons/ayu/` → `icons/ath0/` yet
- Only change user-facing strings (lang.strings) and C++ code
- Style files keep referencing `"ayu/"` path
- Icons stay in `ayu/` directory
- Phase 3: Rename directory when replacement icons ready

**Pros**:
- ✅ Safe - no build breaks
- ✅ Can rebrand now, icons later
- ✅ Internal paths don't affect users

**Cons**:
- ⚠️ "ayu" remains in codebase (but invisible to users)

#### **Option 2: Symlink approach**
```bash
mv Resources/icons/ayu Resources/icons/ath0
ln -s ath0 Resources/icons/ayu  # symlink
```

Both paths work, update references gradually.

**Pros**:
- ✅ Clean rename
- ✅ Backward compatible during transition

**Cons**:
- ⚠️ Symlinks on Windows need admin or Developer Mode
- ⚠️ May confuse build system

#### **Option 3: Atomic rename** (requires ALL icons ready first)
- Prepare ALL replacement ATH0Gram icons first
- Rename directory + update all .style files at once
- Single atomic commit

**Pros**:
- ✅ Clean, complete rebrand

**Cons**:
- ❌ Blocks progress until icons ready
- ⚠️ High risk if any icon missing

---

## Recommendation: HYBRID APPROACH

**Phase 2** (Text Rebrand):
- Keep icon paths as `"ayu/"` in .style files
- Keep directory as `Resources/icons/ayu/`
- Only change visible text and C++ code

**Phase 3** (Icon Replacement - LATER):
When you have replacement icons ready:
1. Replace icon files in `ayu/` directory (same names)
2. OR: Create `ath0/` directory, update .style files atomically
3. Delete extera-related icons
4. Delete donation icons

**Result**: App is fully rebranded to ATH0Gram, but internal icon path is `"ayu/"` (not visible to users)

---

## User Action Required

**For Phase 2 (text rebrand)**: ✅ Nothing - use existing icons

**For Phase 3 (optional icon replacement)**:
- [ ] Design 51 PNG files (17 icons × 3 resolutions)
- [ ] Design 10 .ico app icons (or keep existing with recolor/edit)
- [ ] Formats:
  - PNG: 24-bit RGBA, appropriate sizes
  - ICO: Multi-resolution (16, 32, 48, 256px), 32-bit RGBA

**Tool Recommendations**:
- Inkscape/Illustrator for SVG editing
- GIMP/Photoshop for PNG export
- IcoFX / ImageMagick for .ico creation

---

## Summary

**Build-Critical Files**: 51 PNG + 10 ICO = **61 icon files** must exist
**Safe to Delete Now**: 7 donation SVGs
**Safe to Delete After Disabling Badges**: 12 extera PNGs (4 icons × 3 resolutions)
**Strategy**: Keep `ayu/` path for now, rebrand later when icons ready
