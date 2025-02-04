/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "info/profile/info_profile_members_controllers.h"

#include <rpl/variable.h>
#include "base/weak_ptr.h"
#include "boxes/peers/edit_participants_box.h"
#include "ui/widgets/popup_menu.h"
#include "lang/lang_keys.h"
#include "apiwrap.h"
#include "main/main_session.h"
#include "mainwidget.h"
#include "data/data_channel.h"
#include "data/data_chat.h"
#include "data/data_user.h"
#include "ui/boxes/confirm_box.h"
#include "window/window_session_controller.h"
#include "styles/style_info.h"
#include "data/data_peer_values.h"

namespace Info {
namespace Profile {

MemberListRow::MemberListRow(
	not_null<UserData*> user,
	Type type)
: PeerListRow(user)
, _type(type) {
}

void MemberListRow::setType(Type type) {
	_type = type;
}

QSize MemberListRow::rightActionSize() const {
	return canRemove()
		? QRect(
			QPoint(),
			st::infoMembersRemoveIcon.size()).marginsAdded(
				st::infoMembersRemoveIconMargins).size()
		: QSize();
}

void MemberListRow::rightActionPaint(
		Painter &p,
		int x,
		int y,
		int outerWidth,
		bool selected,
		bool actionSelected) {
	if (_type.canRemove && selected) {
		x += st::infoMembersRemoveIconMargins.left();
		y += st::infoMembersRemoveIconMargins.top();
		(actionSelected
			? st::infoMembersRemoveIconOver
			: st::infoMembersRemoveIcon).paint(p, x, y, outerWidth);
	}
}

// Source from kotatogram
int MemberListRow::adminTitleWidth() const {
	return st::normalFont->width(_type.adminTitle);
}

not_null<UserData*> MemberListRow::user() const {
	return peer()->asUser();
}

// Source from kotatogram
void MemberListRow::paintAdminTitle(
		Painter &p,
		int x,
		int y,
		int outerWidth,
		bool selected) {
	p.drawTextLeft(x, y, outerWidth, _type.adminTitle, adminTitleWidth());
}

void MemberListRow::refreshStatus() {
	if (user()->isBot()) {
		auto seesAllMessages = (user()->botInfo->readsAllHistory || _type.rights != Rights::Normal);
		setCustomStatus(seesAllMessages
			? tr::lng_status_bot_reads_all(tr::now)
			: tr::lng_status_bot_not_reads_all(tr::now));
	} else {
		PeerListRow::refreshStatus();
	}
}

std::unique_ptr<PeerListController> CreateMembersController(
		not_null<Window::SessionNavigation*> navigation,
		not_null<PeerData*> peer) {
	return std::make_unique<ParticipantsBoxController>(
		navigation,
		peer,
		ParticipantsBoxController::Role::Profile);
}

} // namespace Profile
} // namespace Info
