#include "cquestbook.h"
#include "cquestcomment.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

QuestBook::QuestBook()
{
	int i;

	AcolytKill = PotionGift = 0;
	CurseStatus = 2;
	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++) status[i] = 0;
	for (i = 0; i < TOTAL_BOOK_COMMENT; i++) comment[i] = 0;
	MaxHeal = MinPotionGift = false;

	Selector = new Elevator();
}

QuestBook::QuestBook(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height) :
Panel(2, 6, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), NULL, NULL, Str()),
Item(NULL, NULL, Item_width, Item_height, "JOURNAL DES QUETES")
{
	int i;

	AcolytKill = PotionGift = 0;
	CurseStatus = 2;
	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++) status[i] = 0;
	for (i = 0; i < TOTAL_BOOK_COMMENT; i++) comment[i] = 0;
	MaxHeal = MinPotionGift = false;

	Selector = new Elevator(true, false, false, 16, 16, "Sélecteur du journal");
}

QuestBook::~QuestBook()
{
	delete Selector;

	RemoveAllElement(0, true);
	CommentPack.RemoveAllElement(0, true);
}

void QuestBook::IncreaseAcolytKill()
{
	AcolytKill++;

	if (status[WARRIOR_QUEST])
	{
		show();
		Refresh();
	}
}

void QuestBook::Init()
{
	int i;

	Grid[0][0].ColSpan = Grid[1][0].ColSpan = Grid[2][0].ColSpan = -1;
	Grid[0][0].halign = Grid[1][0].halign = Grid[2][0].halign = HALIGN_CENTER;
	Grid[1][0].display = Grid[2][0].display = false;
	Grid[3][0].data = VISUAL_INHERIT;
	Grid[3][0].mosaic = true;

	for (i = 3; i < RowCount; i++)
		AlignColsOnRow(i);

	Panel::Init();

	Grid[3][0].Content = Selector;
	AddForbidden1(Selector);
	*Selector->getDimension() = Axe(16, 176);
	*Selector->GetCage()->getDimension() = Axe(16, 16);

	CommentPack.RemoveAllElement(0, true);
	CommentPack.Initialize(TOTAL_BOOK_COMMENT);
	CommentPack.AddElement(WARRIOR_STEP1, new Object("Le Capitaine m'a demandé de tuer les Acolytes servants de relai au Sorcier. Leur repère se trouve dans les Montagnes de l'Est."));
	CommentPack.AddElement(WARRIOR_STEP2, new Object("Ca y'est, j'ai liquidé les Acolytes! Les créatures environnantes ne sont plus touchées par la malédiction. Il faut que je retourne au Village l'annoncer au Capitaine."));
	CommentPack.AddElement(PRIEST_STEP1, new Object(BuildString("J'ai accepté de devenir le nouveau Prêtre du Village. Je dois pour cela apprendre le sort Soin Vital maximum et ramener %d potions de vie de puissance %d au Sage. Il m'a dit que je trouverai les ingrédients nécessaires au sud-ouest du Village.", TOTAL_POTION_GIFT, POTION_GIFT_POWER)));
	CommentPack.AddElement(PRIEST_STEP2, new Object("Je connais le dernier sort Soin Vital! J'ai donc accompli la première partie de l'apprentissage du Prêtre."));
	CommentPack.AddElement(PRIEST_STEP3, new Object("J'ai créé les potions nécessaires pour l'accomplissement de l'apprentissage du Prêtre. Je peux aller les donner au Sage."));
	CommentPack.AddElement(FINAL_STEP1, new Object("Le Chef du Village m'a convoqué pour me demander de tuer le Sorcier. Tant qu'à faire j'ai accepté... Il paraît qu'il a des gardiens féroces et qu'il est indestructible! Mais il existe un moyen de le détruire et je le trouverai!"));
	CommentPack.AddElement(FINAL_STEP2, new Object("J'ai tué le Sorcier, la malédiction va enfin disparaître. Youpi!!!"));
}

void QuestBook::initAni()
{
	Selector->addAni(IDB_ELEVATOR_BKG, "", 1, 16, 16, -1, 0);
	Selector->GetCage()->addAni(IDB_ELEVATOR, "", 1, 16, 16, -1, 0);
	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void QuestBook::restoreAni()
{
	Panel::restoreAni();

	Selector->restoreAni();
}

void QuestBook::destroyAni()
{
	Panel::destroyAni();

	Selector->destroyAni();
}

void QuestBook::Start()
{
	int i;

	AcolytKill = PotionGift = 0;
	CurseStatus = 2;
	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++) status[i] = 0;
	MaxHeal = MinPotionGift = false;

	for (i = 3; i < RowCount; i++)
		((Text*)Grid[i][1].Content)->SetContent(NULL);

	RemoveAllElement(0, true);

	Refresh();
}

void QuestBook::AddComment(int day, bool ShowIt, BOOK_COMMENT MsgID, MsgReporter *pMsgReporter)
{
	int i, quest;
	QuestComment *Comment;

	switch (MsgID)
	{
	case WARRIOR_STEP1:
		quest = WARRIOR_QUEST;
		break;
	case WARRIOR_STEP2:
		quest = WARRIOR_QUEST;
		break;
	case PRIEST_STEP1:
		quest = PRIEST_QUEST;
		break;
	case PRIEST_STEP2:
		quest = PRIEST_QUEST;
		break;
	case PRIEST_STEP3:
		quest = PRIEST_QUEST;
		break;
	case FINAL_STEP1:
		quest = FINAL_QUEST;
		break;
	case FINAL_STEP2:
		quest = FINAL_QUEST;
		break;
	default:
		quest = GUARD_QUEST;
		break;
	}

	Comment = new QuestComment(quest, day, *CommentPack.GetElement(MsgID)->getName());

	if (comment[MsgID])
	{
		i = SearchElementByName(Comment->GetComment(), false);
		if (i != -1)
		{
			AddElement(i, Comment);
			OrderBy(order, NULL);
		}
		else
			AddElement(Comment);
	}
	else
	{
		comment[MsgID] = 1;
		AddElement(Comment);
	}

	if (ShowIt) show();
	Refresh();

	if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, "Vous avez mis votre journal à jour", ACTION_MSG_COLOR);
}

bool QuestBook::Refresh()
{
	int i, CommentID;

	// Cellules à afficher lorsque la quête du guerrier est en cours
	switch (status[WARRIOR_QUEST])
	{
	case 0:
		Grid[1][0].display = false;
		break;
	case 3:
		Grid[1][0].display = false;
		break;
	default:
		Grid[1][0].display = true;
		break;
	}

	// Cellules à afficher lorsque la quête du prêtre est en cours
	switch (status[PRIEST_QUEST])
	{
	case 0:
		Grid[2][0].display = false;
		break;
	case 3:
		Grid[2][0].display = false;
		break;
	default:
		Grid[2][0].display = true;
		break;
	}

	// Affichage du texte
	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, BuildString("Acolytes Relais tués: %d", AcolytKill), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, BuildString("Potions préparées: %d/%d", PotionGift, TOTAL_POTION_GIFT), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	// Défilement des messages
	for (i = 3; i < RowCount; i++)
	{
		CommentID = i - 3 + Selector->GetStage(ElementCount-1);
		if ((ElementCount > CommentID) && (ElementCount > 0) && (Grid[i][1].posy + Grid[i][1].height <= dimension.y))
		{
			Grid[i][1].display = true;
			((Text*)Grid[i][1].Content)->Update(280, RGB(192, 192, 192), QuestComment::getQuestCommentElement(CommentID, this)->GetColor(), GetElement(CommentID)->getName());
		}
		else
		{
			Grid[i][1].display = false;
			((Text*)Grid[i][1].Content)->SetContent(NULL);
		}
	}

	return Panel::Refresh();
}

bool QuestBook::generate(bool waited, Cursor *cursor)
{
	if (Selector->Slide(cursor))
		Refresh();

	return Panel::generate(waited, cursor);
}

int QuestBook::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_DAY:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = QuestComment::getQuestCommentElement(i, this)->GetDay();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		case ORDER_BY_QUEST:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = QuestComment::getQuestCommentElement(i, this)->GetQuest();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		case ORDER_BY_COMMENT:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->alpha = QuestComment::getQuestCommentElement(i, this)->GetComment();
			result = OrderElements(ORDER_ALPHA);
			if (!result) return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}
