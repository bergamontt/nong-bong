#include "BankCardList.h"
#include "feature/card/ICardService.h"
#include <QPainter>
#include <QStyleOption>
#include <QResizeEvent>
#include <QEasingCurve>

using namespace std;

BankCardList::BankCardList(IContext& context, int userId, QWidget* parent)
	: QWidget(parent), _context(context), _userId(userId)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setMinimumSize(300, 200);

	_leftCard = new BankCardWidget(this);
	_leftCard->setVisible(false);
	_centerCard = new BankCardWidget(this);
	_centerCard->setVisible(false);
	_rightCard = new BankCardWidget(this);
	_rightCard->setVisible(false);

	_prevBtn = new QPushButton(QStringLiteral("<"), this);
	_prevBtn->setFocusPolicy(Qt::NoFocus);
	_prevBtn->setToolTip(tr("Previous card"));
	_prevBtn->setFlat(true);
	_prevBtn->setFixedSize(40, 40);
	_prevBtn->setStyleSheet("background-color: rgba(0,0,0,192); color: white; border: none; border-radius: 20px;");
	connect(_prevBtn, &QPushButton::clicked, this, &BankCardList::onPrevClicked);

	_nextBtn = new QPushButton(QStringLiteral(">"), this);
	_nextBtn->setFocusPolicy(Qt::NoFocus);
	_nextBtn->setToolTip(tr("Next card"));
	_nextBtn->setFlat(true);
	_nextBtn->setFixedSize(40, 40);
	_nextBtn->setStyleSheet("background-color: rgba(0,0,0,192); color: white; border: none; border-radius: 20px;");
	connect(_nextBtn, &QPushButton::clicked, this, &BankCardList::onNextClicked);

	_emptyLabel = new QLabel(tr("No cards available"), this);
	_emptyLabel->setAlignment(Qt::AlignCenter);
	_emptyLabel->setVisible(false);

	_anim = new QPropertyAnimation(this, "carouselShift", this);
	_anim->setDuration(300);
	_anim->setEasingCurve(QEasingCurve::InOutCubic);
	connect(_anim, &QPropertyAnimation::finished, this, &BankCardList::onAnimationFinished);
}
void BankCardList::refresh()
{
	updateCards();
	updateVisibleWidgets();
	layoutWidgets();
}

void BankCardList::updateCards()
{
	const ICardService& service = _context.cardService();
	_cards = service.getAllActiveCardsByUserId(_userId);
	vector<Card> blocked = service.getAllBlockedCardsByUserId(_userId);
	_cards.insert(_cards.end(), blocked.begin(), blocked.end());
	if (_cards.empty())
		_selectedIndex = -1;
	else
		_selectedIndex = 0;
}

std::optional<Card> BankCardList::getSelectedCard()
{
	if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(_cards.size()))
		return _cards[_selectedIndex];
	return nullopt;
}

void BankCardList::onPrevClicked()
{
	if (_cards.empty()) 
		return;
	if (_anim->state() == QAbstractAnimation::Running) 
		return;
	if (!(_selectedIndex > 0)) 
		return;

	QRect avail = contentsRect();
	const int w = avail.width();
	const int h = avail.height();

	const double centerWRatio = 0.60;
	const double centerHRatio = 0.60;
	const int gap = qMax(16, w / 40);
	const int centerW = static_cast<int>(w * centerWRatio);
	const int centerH = static_cast<int>(h * centerHRatio);
	const int centerX = (w - centerW) / 2;
	const int leftX = centerX - (centerW + gap);
	const int step = centerX - leftX;

	int nextIndex = _selectedIndex - 1;
	if (nextIndex == _selectedIndex) 
		return;
	_pendingIndex = nextIndex;
	_leftCard->setCard(_cards[_pendingIndex]);
	_leftCard->setVisible(true);
	_leftCard->raise();

	_anim->stop();
	_anim->setStartValue(0);
	_anim->setEndValue(step);
	_anim->start();
}

void BankCardList::onNextClicked()
{
	if (_cards.empty()) 
		return;
	if (_anim->state() == QAbstractAnimation::Running) 
		return;
	if (!(_selectedIndex + 1 < static_cast<int>(_cards.size()))) 
		return;

	QRect avail = contentsRect();
	const int w = avail.width();
	const int h = avail.height();

	const double centerWRatio = 0.60;
	const double centerHRatio = 0.60;
	const int gap = qMax(16, w / 40);
	const int centerW = static_cast<int>(w * centerWRatio);
	const int centerH = static_cast<int>(h * centerHRatio);
	const int centerX = (w - centerW) / 2;
	const int rightX = centerX + centerW + gap;
	const int step = rightX - centerX;

	int nextIndex = _selectedIndex + 1;
	if (nextIndex == _selectedIndex) 
		return;
	_pendingIndex = nextIndex;
	_rightCard->setCard(_cards[_pendingIndex]);
	_rightCard->setVisible(true);
	_rightCard->raise();

	_anim->stop();
	_anim->setStartValue(0);
	_anim->setEndValue(-step);
	_anim->start();
}

void BankCardList::onAnimationFinished()
{
	if (_pendingIndex >= 0 && _pendingIndex < static_cast<int>(_cards.size()))
		_selectedIndex = _pendingIndex;
	_pendingIndex = -1;
	_carouselShift = 0;
	setCarouselShift(0);
	updateVisibleWidgets();
	layoutWidgets();
	if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(_cards.size()))
		emit selectedCardChanged(_cards[_selectedIndex]);
}
void BankCardList::updateVisibleWidgets()
{
	const bool has = !_cards.empty();
	_emptyLabel->setVisible(!has);
	if (!has || _selectedIndex < 0 || _selectedIndex >= static_cast<int>(_cards.size()))
	{
		_leftCard->setVisible(false);
		_centerCard->setVisible(false);
		_rightCard->setVisible(false);
		if (_prevBtn) 
		{ 
			_prevBtn->setVisible(false); 
			_prevBtn->setEnabled(false); 
		}
		if (_nextBtn) 
		{ 
			_nextBtn->setVisible(false); 
			_nextBtn->setEnabled(false); 
		}
		return;
	}

	_centerCard->setCard(_cards[_selectedIndex]);
	_centerCard->setVisible(true);
	int leftIdx = _selectedIndex - 1;
	if (leftIdx >= 0) 
	{ 
		_leftCard->setCard(_cards[leftIdx]); 
		_leftCard->setVisible(true);
		_leftCard->lower(); 
	}
	else 
	{ 
		_leftCard->setVisible(false); 
	}
	int rightIdx = _selectedIndex + 1;
	if (rightIdx < static_cast<int>(_cards.size())) 
	{ 
		_rightCard->setCard(_cards[rightIdx]); 
		_rightCard->setVisible(true); 
		_rightCard->lower(); 
	}
	else 
	{ 
		_rightCard->setVisible(false); 
	}

	if (_prevBtn)
	{
		bool isActive = has && _cards.size() > 1 && _selectedIndex > 0;
		_prevBtn->setVisible(isActive);
		_prevBtn->setEnabled(isActive);
	}
	if (_nextBtn)
	{
		bool isActive = has && _cards.size() > 1
			&& (_selectedIndex + 1) < static_cast<int>(_cards.size());
		_nextBtn->setVisible(isActive);
		_nextBtn->setEnabled(isActive);
	}

	_centerCard->raise();
	_prevBtn->raise();
	_nextBtn->raise();
}

void BankCardList::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BankCardList::showEvent(QShowEvent* event) {
	QWidget::showEvent(event);
	refresh();
}

void BankCardList::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	layoutWidgets();
}

void BankCardList::layoutWidgets()
{
	QRect avail = contentsRect();
	const int w = avail.width();
	const int h = avail.height();
	if (w == 0 || h == 0) 
		return;

	const double centerWRatio = 0.60;
	const double centerHRatio = 0.60;
	const int gap = qMax(16, w / 40);
	const int centerW = static_cast<int>(w * centerWRatio);
	const int centerH = static_cast<int>(h * centerHRatio);
	const int centerX = (w - centerW) / 2;
	const int centerY = (h - centerH) / 2;
	const int leftX = centerX - (centerW + gap);
	const int rightX = centerX + centerW + gap;
	const int shift = _carouselShift;

	if (_centerCard && _centerCard->isVisible())
		_centerCard->setGeometry(centerX + shift, centerY, centerW, centerH);
	if (_leftCard && _leftCard->isVisible())
		_leftCard->setGeometry(leftX + shift, centerY, centerW, centerH);
	if (_rightCard && _rightCard->isVisible())
		_rightCard->setGeometry(rightX + shift, centerY, centerW, centerH);

	const int btnW = _prevBtn ? _prevBtn->width() : 40;
	const int btnH = _prevBtn ? _prevBtn->height() : 40;
	const int btnY = (h - btnH) / 2;
	int prevX = 6;
	int nextX = w - btnW - 6;
	if (_prevBtn) 
		_prevBtn->move(prevX, btnY);
	if (_nextBtn) 
		_nextBtn->move(nextX, btnY);
	if (_emptyLabel) 
		_emptyLabel->setGeometry(0, 0, w, h);
}

int BankCardList::carouselShift() const
{
	return _carouselShift;
}

void BankCardList::setCarouselShift(int s)
{
	_carouselShift = s;
	layoutWidgets();
}