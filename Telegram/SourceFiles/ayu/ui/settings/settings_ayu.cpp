// This is the source code of AyuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2025
#include "settings_ayu.h"

#include "lang_auto.h"
#include "settings_ayu_utils.h"
#include "ayu/ayu_settings.h"
#include "boxes/peer_list_box.h"
#include "filters/settings_filters_list.h"
#include "settings/settings_common.h"
#include "styles/style_settings.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
#include "ui/wrap/vertical_layout.h"
#include "window/window_session_controller.h"

namespace Settings {

rpl::producer<QString> AyuGhost::title() {
	return rpl::single(QString("ATH0Gram"));
}

AyuGhost::AyuGhost(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
	: Section(parent) {
	setupContent(controller);
}

void SetupGhostModeToggle(not_null<Ui::VerticalLayout*> container) {
	auto *settings = &AyuSettings::getInstance();

	AddSubsectionTitle(container, tr::ath0_GhostEssentialsHeader());

	std::vector checkboxes{
		NestedEntry{
			tr::ath0_DontReadMessages(tr::now), !settings->sendReadMessages, [=](bool enabled)
			{
				AyuSettings::set_sendReadMessages(!enabled);
				AyuSettings::save();
			}
		},
		NestedEntry{
			tr::ath0_DontReadStories(tr::now), !settings->sendReadStories, [=](bool enabled)
			{
				AyuSettings::set_sendReadStories(!enabled);
				AyuSettings::save();
			}
		},
		NestedEntry{
			tr::ath0_DontSendOnlinePackets(tr::now), !settings->sendOnlinePackets, [=](bool enabled)
			{
				AyuSettings::set_sendOnlinePackets(!enabled);
				AyuSettings::save();
			}
		},
		NestedEntry{
			tr::ath0_DontSendUploadProgress(tr::now), !settings->sendUploadProgress, [=](bool enabled)
			{
				AyuSettings::set_sendUploadProgress(!enabled);
				AyuSettings::save();
			}
		},
		NestedEntry{
			tr::ath0_SendOfflinePacketAfterOnline(tr::now), settings->sendOfflinePacketAfterOnline, [=](bool enabled)
			{
				AyuSettings::set_sendOfflinePacketAfterOnline(enabled);
				AyuSettings::save();
			}
		},
	};

	AddCollapsibleToggle(container, tr::ath0_GhostModeToggle(), checkboxes, true);
}

void SetupGhostEssentials(
	not_null<Ui::VerticalLayout*> container,
	not_null<AyuSettings::AyuGramSettings*> settings,
	not_null<rpl::variable<bool>*> markReadAfterActionVal,
	not_null<rpl::variable<bool>*> useScheduledMessagesVal) {
	SetupGhostModeToggle(container);


	AddButtonWithIcon(
		container,
		tr::ath0_MarkReadAfterAction(),
		st::settingsButtonNoIcon
	)->toggleOn(
		markReadAfterActionVal->value()
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->markReadAfterAction);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_markReadAfterAction(enabled);
			if (enabled) {
				AyuSettings::set_useScheduledMessages(false);
				useScheduledMessagesVal->force_assign(false);
			}

			AyuSettings::save();
		},
		container->lifetime());
	AddSkip(container);
	AddDividerText(container, tr::ath0_MarkReadAfterActionDescription());
}

void SetupScheduleMessages(
	not_null<Ui::VerticalLayout*> container,
	not_null<AyuSettings::AyuGramSettings*> settings,
	not_null<rpl::variable<bool>*> markReadAfterActionVal,
	not_null<rpl::variable<bool>*> useScheduledMessagesVal) {
	AddSkip(container);
	AddButtonWithIcon(
		container,
		tr::ath0_UseScheduledMessages(),
		st::settingsButtonNoIcon
	)->toggleOn(
		useScheduledMessagesVal->value()
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->useScheduledMessages);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_useScheduledMessages(enabled);
			if (enabled) {
				AyuSettings::set_markReadAfterAction(false);
				markReadAfterActionVal->force_assign(false);
			}

			AyuSettings::save();
		},
		container->lifetime());
	AddSkip(container);
	AddDividerText(container, tr::ath0_UseScheduledMessagesDescription());
}

void SetupSendWithoutSound(not_null<Ui::VerticalLayout*> container) {
	auto *settings = &AyuSettings::getInstance();

	AddSkip(container);
	AddButtonWithIcon(
		container,
		tr::ath0_SendWithoutSoundByDefault(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->sendWithoutSound)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->sendWithoutSound);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_sendWithoutSound(enabled);
			AyuSettings::save();
		},
		container->lifetime());
	AddSkip(container);
	AddDividerText(container, tr::ath0_SendWithoutSoundByDefaultDescription());
}

void SetupSpyEssentials(not_null<Ui::VerticalLayout*> container) {
	auto *settings = &AyuSettings::getInstance();

	AddSubsectionTitle(container, tr::ath0_SpyEssentialsHeader());

	AddButtonWithIcon(
		container,
		tr::ath0_SaveDeletedMessages(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->saveDeletedMessages)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->saveDeletedMessages);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_saveDeletedMessages(enabled);
			AyuSettings::save();
		},
		container->lifetime());

	AddButtonWithIcon(
		container,
		tr::ath0_SaveMessagesHistory(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->saveMessagesHistory)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->saveMessagesHistory);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_saveMessagesHistory(enabled);
			AyuSettings::save();
		},
		container->lifetime());

	AddSkip(container);
	AddDivider(container);
	AddSkip(container);

	AddButtonWithIcon(
		container,
		tr::ath0_MessageSavingSaveForBots(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->saveForBots)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->saveForBots);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_saveForBots(enabled);
			AyuSettings::save();
		},
		container->lifetime());
}

void SetupOther(not_null<Ui::VerticalLayout*> container) {
	auto *settings = &AyuSettings::getInstance();

	AddSubsectionTitle(container, tr::ath0_MessageSavingOtherHeader());

	AddButtonWithIcon(
		container,
		tr::ath0_LocalPremium(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->localPremium)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->localPremium);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_localPremium(enabled);
			AyuSettings::save();
		},
		container->lifetime());

	AddButtonWithIcon(
		container,
		tr::ath0_DisableAds(),
		st::settingsButtonNoIcon
	)->toggleOn(
		rpl::single(settings->disableAds)
	)->toggledValue(
	) | rpl::filter(
		[=](bool enabled)
		{
			return (enabled != settings->disableAds);
		}) | start_with_next(
		[=](bool enabled)
		{
			AyuSettings::set_disableAds(enabled);
			AyuSettings::save();
		},
		container->lifetime());
}

void AyuGhost::setupContent(not_null<Window::SessionController*> controller) {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

	const auto settings = &AyuSettings::getInstance();
	const auto markReadAfterActionVal = content->lifetime().make_state<rpl::variable<bool>>(
		settings->markReadAfterAction);
	const auto useScheduledMessagesVal = content->lifetime().make_state<rpl::variable<bool>>(
		settings->useScheduledMessages);

	AddSkip(content);

	SetupGhostEssentials(content, settings, markReadAfterActionVal, useScheduledMessagesVal);
	SetupScheduleMessages(content, settings, markReadAfterActionVal, useScheduledMessagesVal);
	SetupSendWithoutSound(content);

	AddSkip(content);
	SetupSpyEssentials(content);

	AddSkip(content);
	AddDivider(content);
	AddSkip(content);

	SetupOther(content);
	AddSkip(content);

	ResizeFitChild(this, content);
}

} // namespace Settings
