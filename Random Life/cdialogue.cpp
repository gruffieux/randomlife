#include "cdialogue.h"
#include "caskforsave.h"
#include "cgauge.h"
#include "ccharinfo.h"
#include "cinventory.h"
#include "cquestbook.h"
#include "cworldtime.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern MsgReporter EventsReporter;
extern MiniEvents ActionReporter;
extern AskForSave *PSave;
extern Gauge *PGauge;
extern CharInfo *PCharInfo;
extern Inventory *PInventory;
extern QuestBook *PBook;
extern WorldTime *PWorldTime;
extern Pickup *Authorization, *MagicCircle, *MindProtector;
extern Weapon *Daggy[TOTAL_PICKUP_WEAPON_DAGGY];
extern Armor *MagicianDress[TOTAL_PICKUP_ARMOR_MAGICIAN_DRESS];
extern Potion *DiscretionFlow[TOTAL_PICKUP_POTION_DF];

Dialogue::Dialogue()
{
	int i;

	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++)
		step[i] = 0;

	Speaker = NULL;
	Listener = NULL;
}

Dialogue::Dialogue(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(2, 3, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "DIALOGUE")
{
	int i;

	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++)
		step[i] = 0;

	Speaker = NULL;
	Listener = NULL;
}

bool Dialogue::show()
{
	if (Panel::show())
	{
		InitMsg();
		return true;
	}

	return false;
}

bool Dialogue::hide()
{
	if (Panel::hide())
	{
		DestroyMsg();
		return true;
	}

	return false;
}

void Dialogue::Init()
{
	AlignCols();
	AlignRows();

	Grid[1][1].data = VISUAL;
	Grid[2][1].data = VISUAL;

	Grid[1][1].PicFrame = 0;
	Grid[2][1].PicFrame = 0;

	Panel::Init();

	AddForbidden1(GetVisualData(1, 1));
	AddForbidden1(GetVisualData(1, 2));
}

void Dialogue::initAni()
{
	GetVisualData(1, 1)->addAni(IDB_SELECT_BUTTONS, "", 1, 16, 16, -1, 90);
	GetVisualData(1, 2)->addAni(IDB_SELECT_BUTTONS, "", 1, 16, 16, -1, 90);
	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void Dialogue::restoreAni()
{
	Panel::restoreAni();

	GetVisualData(1, 1)->restoreAni();
	GetVisualData(1, 2)->restoreAni();
}

void Dialogue::destroyAni()
{
	Panel::destroyAni();

	GetVisualData(1, 1)->destroyAni();
	GetVisualData(1, 2)->destroyAni();
}

void Dialogue::InitMsg()
{
	List ModelMsg;

	DestroyMsg();

	ModelMsg.AddElement(new Object("Tiens, voici une autorisation pour parler au Chef du village en personne. Va le voir, il t'expliquera ce qu'il faut faire."));
	ModelMsg.AddElement(new Object(BuildString("Je n'ai plus rien à te demander, tu as accomplis ton devoir envers moi, Merci encore %s!", Listener->GetDisplayedName()->Get())));
	ModelMsg.AddElement(new Object("Ah? ..."));
	ModelMsg.AddElement(new Object("D'accord!"));
	ModelMsg.AddElement(new Object("Merci, au revoir!"));
	ModelMsg.AddElement(new Object("Mais de rien!"));
	ModelMsg.AddElement(new Object("Je m'en fous! Ciao!"));
	ModelMsg.AddElement(new Object("Tu parles! Vive la pénitence ouais!"));
	ModelMsg.AddElement(new Object("Non merci, je ne tiens pas à lui parler."));
	ModelMsg.AddElement(new Object("Ok."));

	switch (Speaker->GetPNJQuest())
	{
	case WARRIOR_QUEST:
		QuestMsg.AddElement(new Object(BuildString("Salut %s, alors tu t'est décidé à te bouger un peu les fesses pour notre village? Voilà une bonne décision! Comme tu le sais sans doute, les habitants du village subissent de plus en plus d'attaque par les créatures qui rôdent dans les terres... Je dois absolument améliorer la situation et j'ai besoin d'un homme...fort...très fort même. Penses-tu pouvoir m'aider?", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object("Bon, bien... Je t'explique le topo. Le Grand Sorcier qui maintient la malédiction, a à son service des Acolytes dans les montagnes de l'Est, qui l'assistent à la propagation et l'extension du fléau dans nos terres. Je veux que tu détruisent ces Acolytes, leur disparition réduira la puissance de la malédiction... Mais attention! Ce sont des adeptes de magie et certains sont tout aussi puissant que nos plus grand Sages!"));
		QuestMsg.AddElement(new Object(BuildString("C'est tout %s. Aller dans les montages de l'Est et détruire les Acolytes, telle est la tâche que je te demande... Accepte-tu de l'accomplir?", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object("Tiens, voici un collier ainsi qu'un anneau, ce sont des bijoux enchantés et ils t'aideront à remplir cette mission à bien. Le collier protége des dégats magiques. L'anneau permet d'invoquer une arme spéciale, bien utile contre certains Magiciens... Je te préviens, il faut être très fort pour porter ces objets! Maintenant part, bonne chance!"));
		QuestMsg.AddElement(new Object(BuildString("Eh bien %s, qu'attends-tu? c'est aux montages, à l'Est que ça se passe!", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object(BuildString("Bravo %s! Tu as fait preuve de bien du courage en accomplissant cette quête. Je ne te consédérerai plus comme un bête villageois mais comme un grand guerrier à présent! Grâce à toi le village sera plus tranquille maintenant. Mais nos terres ne sont pas encore paisibles...", Listener->GetDisplayedName()->Get())));
		if (PBook->GetStatus(PRIEST_QUEST) == 3)
			QuestMsg.AddElement(new Object("Le Sage m'a annoncé que tu étais notre nouveau Prêtre. Décidément tu es un sacré gaillard! Si tu es d'accord, il faudrait encore faire une dernière chose pour nous..."));
		else
			QuestMsg.AddElement(new Object("Je te suggère d'aller parler au Sage qui à pas mal de souçis..."));
		QuestMsg.AddElement(ModelMsg.GetElement(0));
		QuestMsg.AddElement(ModelMsg.GetElement(1));
		AnswerYesMsg.AddElement(new Object("Oui, dites-m'en plus!"));
		AnswerYesMsg.AddElement(new Object("Continuez..."));
		AnswerYesMsg.AddElement(new Object("Oui, ça marche!"));
		AnswerYesMsg.AddElement(new Object("Merci, au revoir!"));
		AnswerYesMsg.AddElement(new Object("Oui oui j'y cours!"));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(2));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(3));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(4));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(5));
		AnswerNoMsg.AddElement(new Object("Non, je suis faible et trouillard. A une prochaine!"));
		AnswerNoMsg.AddElement(new Object("Mouais bof, j'ai mieux à faire en fait, bye!"));
		AnswerNoMsg.AddElement(new Object("En fait, je viens de me rappeler que j'étais pris ce soir. Tempis, une prochaine fois..."));
		AnswerNoMsg.AddElement(new Object("Non merci, je saurai me débrouiller sans ses babioles! Allez ciao!"));
		AnswerNoMsg.AddElement(new Object("Heuu... tout bien réfléchis je préfère laisser tomber, j'ai une femme et des enfants... désolé!"));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(6));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(7));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(8));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(9));
		break;
	case PRIEST_QUEST:
		QuestMsg.AddElement(new Object(BuildString("Bonjour mon cher %s, quel plaisir de te rencontrer. Qu'entens-je, tu viens donner ton aide pour le village? Quelle générosité et tu tombes bien car il est arrivé un grand malheur!", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object("Hier notre bon Prêtre est allé faire sa coeuillette habituelles pour ses potions. Il s'est aventuré un peu loin dans les terres et a été attaqué par des créatures folles furieuses... Des villageois l'on trouvé et ramené chez lui mais malgré mes tentatvies pour le guérir, les coups cruels de ces monstres ont eu raison de lui, il est mort ce matin..."));
		QuestMsg.AddElement(new Object("Oui, c'est une grande perte pour le village car nous n'avons plus de soigneur et comme tu le sais sans doute, les villageois subissent sans cesse des attaques! Il nous faut donc un nouveau Prêtre... C'est un métier qui s'aquiert en devenant un magicien soigneur. Il faut aussi savoir créer des potions magiques pour soigner de manière durable les malades et blessés. L'apprentissage inclut donc la découverte d'ingrédients et la connaissance de leur composé chimique. Cela t'intéresse t'il?"));
		QuestMsg.AddElement(new Object(BuildString("Merci %s, je m'incline avec respect pour ton dévouement. Pars donc dans les terres, deviens un grand soigneur en combattant et en te défandant contre les créatures et ramène nous %d potions magiques de vie de puissance %d au moin. Le Sud-Ouest des terres est riche en fleurs possédants des vertus spéciaux, tu trouveras ce qu'il te faut là-bas.", Listener->GetDisplayedName()->Get(), TOTAL_POTION_GIFT, POTION_GIFT_POWER)));
		QuestMsg.AddElement(new Object(BuildString("Tu n'est pas encore Prêtre %s. Reviens quand tu connaîtra le sort Soin Vital maximum et aura fait %d potions de vie de puissance %d au moin.", Listener->GetDisplayedName()->Get(), TOTAL_POTION_GIFT, POTION_GIFT_POWER)));
		QuestMsg.AddElement(new Object(BuildString("Félicitations %s, tu es maintenant notre nouveau Prêtre! As-tu aussi les %d potions de vie de puissance %d?", Listener->GetDisplayedName()->Get(), TOTAL_POTION_GIFT, POTION_GIFT_POWER)));
		QuestMsg.AddElement(new Object("Un grand merci pour ta générosité et ton dévouement, nous avions besoin d'un homme comme toi. Tiens, je t'offre cette robe ainsi qu'une Daggy. Chaque puissant magicien possède cet équipement, le Grand mage, le Prètre, ainsi que le Sorcier... Eh oui! Ce vil était un sage homme autrefois... Avec lui vivant aujourd'hui, nos ennuis sont loin d'être terminés..."));
		if (PBook->GetStatus(WARRIOR_QUEST) == 3)
			QuestMsg.AddElement(new Object("J'ai eu vent du Capitaine que tu l'as également aidé. Ton aide est inestimable! Oserais-je te demander une dernière faveur? Pas la moindre je te l'accorde..."));
		else
			QuestMsg.AddElement(new Object("Je te conseil d'aller parler au Capitaine. Il a sans doute besoin de ton aide..."));
		QuestMsg.AddElement(ModelMsg.GetElement(0));
		QuestMsg.AddElement(ModelMsg.GetElement(1));
		AnswerYesMsg.AddElement(new Object("Que s'est t'il passé???"));
		AnswerYesMsg.AddElement(new Object("J'en suis désolé et horrifé en même temps!"));
		AnswerYesMsg.AddElement(new Object("Bien sûr! Je deviendrai votre nouveau Prêtre."));
		AnswerYesMsg.AddElement(new Object("D'accord, je reviendrai avec cela, faites moi confiance!"));
		AnswerYesMsg.AddElement(new Object("Je comprend, j'y retourne alors."));
		AnswerYesMsg.AddElement(new Object("Oui, les voilà."));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(2));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(3));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(4));
		AnswerYesMsg.AddElement(ModelMsg.GetElement(5));
		AnswerNoMsg.AddElement(new Object("Ce n'est pas mes oignons, au revoir!"));
		AnswerNoMsg.AddElement(new Object("Bah, un Prêtre c'est pas d'une grande utilité, c'est donc pas une grande perte..."));
		AnswerNoMsg.AddElement(new Object("En fait, je suis assez surbooké ces temps et n'ai pas tellement le temps de suivre une formation. Une autre fois peut-être..."));
		AnswerNoMsg.AddElement(new Object("Oulala ça en fait du boulot, je crois que je vais réfléchir."));
		AnswerNoMsg.AddElement(new Object("Arf, c'est trop dure je préfère laisser tomber, désolé..."));
		AnswerNoMsg.AddElement(new Object("Non, pas encore..."));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(6));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(7));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(8));
		AnswerNoMsg.AddElement(ModelMsg.GetElement(9));
		break;
	case GUARD_QUEST:
		QuestMsg.AddElement(new Object("Halte! On ne passe pas ici. Il faut une autorisation pour entrer et parler au chef."));
		QuestMsg.AddElement(new Object("Vous pouvez entrer Sire, le chef vous attend."));
		if (Listener->GetPocket(OTHERS)->SearchElement(Authorization) != -1)
			AnswerYesMsg.AddElement(new Object("Là voici."));
		else
			AnswerYesMsg.AddElement(new Object("Dommage."));
		AnswerYesMsg.AddElement(new Object("Merci."));
		AnswerNoMsg.AddElement(new Object("Je ne fais que visiter, je me fiche de voir le chef."));
		AnswerNoMsg.AddElement(new Object("Mais qu'est ce que je fais ici moi, je devrais déjà être rentré pour manger!"));
		break;
	case FINAL_QUEST:
		QuestMsg.AddElement(new Object(BuildString("Salutation %s! Hum, J'ai eu vent des tes succès et j'ai demandé à ce que tu vienne me voir personnelement... hum... pour accomplir une dernière tâche...", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object("Hum, il faut absolument éliminer ce sorcier! Et je pense que tu es le seul à en être capable ici... Le sort du village est donc entre tes mains et je te supplie d'accepter de nous libérer!"));
		QuestMsg.AddElement(new Object("Super, t'es le meilleur tu sais! Alors voilà... Hum... il faut que tu atteigne le fort du sorcier par le Désert Glacé. Là-bas il y a des chiens maléfiques qui gardent l'entrée! Mais avec tes pouvoirs et ta force tu dois pouvoir arriver à les térasser... Hum!"));
		QuestMsg.AddElement(new Object("Quand tu auras éliminé ses gardiens, il ne restera que le sorcier... Sais-tu qu'il est indestructible? Héhéhé épatant non? Mais ne t'inquiète pas, ce n'est que supercherie de sorcier et il y a sûrement un moyen de la contrer...! Hum, à toi de découvrir ce moyen... Tiens, prend cette potion d'invisibilité, elle peut t'être utile!"));
		QuestMsg.AddElement(new Object(BuildString("Je compte sur toi %s! Le fort est tout au nord, tu dois traverser le Désert Glacé. Bonne chance... Hum", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object(BuildString("Eh bien je m'incline devant toi... Hum! %s, tu nous as libérer et le village connaîtra enfin la paix. Sois infiniment remercié! La place de chef du village te revient entièrement... Hum... à ma mort bien sûre. En attendant, profite de cette paix et de ta liberté tant qu'elle dure!", Listener->GetDisplayedName()->Get())));
		QuestMsg.AddElement(new Object("Tu en as fais assez! Va te divertir maitenant..."));
		AnswerYesMsg.AddElement(new Object("Je t'écoute mon chef."));
		AnswerYesMsg.AddElement(new Object("Bien sûr voyons!"));
		AnswerYesMsg.AddElement(new Object("Oui..."));
		AnswerYesMsg.AddElement(new Object("Merci."));
		AnswerYesMsg.AddElement(new Object("A tout de suite, ou à jamais!"));
		AnswerYesMsg.AddElement(new Object("Je suis très honoré..."));
		AnswerYesMsg.AddElement(new Object("Faisons la fête!!!"));
		AnswerNoMsg.AddElement(new Object("Rhoo j'en ai déjà assez fais! Désolé mais je me sens las, il faut que je me repose."));
		AnswerNoMsg.AddElement(new Object("Je crains d'avoir moin d'estime pour moi que vous. Désolé je ne suis pas celui que vous croyez."));
		AnswerNoMsg.AddElement(new Object("Stop, je t'arrête tout de suite! Premièrement je ne suis pas Superman... Deuxièmement j'ai horreur des chiens! Alors salut."));
		AnswerNoMsg.AddElement(new Object("Non merci, je ne boirai jamais un truc pareil!"));
		AnswerNoMsg.AddElement(new Object("Inutile! Je n'en suis pas capable, j'abandonne"));
		AnswerNoMsg.AddElement(new Object("Désolé, je refuse d'avoir une telle responsabilité, je veux garder une vie modeste."));
		AnswerNoMsg.AddElement(new Object("Rendez-vous au bar!"));
		break;
	}

	ModelMsg.RemoveAllElement();
}

void Dialogue::DestroyMsg()
{
	QuestMsg.RemoveAllElement(0, true);
	AnswerYesMsg.RemoveAllElement(0, true);
	AnswerNoMsg.RemoveAllElement(0, true);
}

void Dialogue::NextStep()
{
	if (step[Speaker->GetPNJQuest()] == QuestMsg.GetElementCount() - 1)
		hide();

	if ((step[Speaker->GetPNJQuest()] < QuestMsg.GetElementCount() - 1) && (step[Speaker->GetPNJQuest()] != 4))
		if (((((Speaker->GetPNJQuest() != WARRIOR_QUEST) | (step[WARRIOR_QUEST] != 6)) && ((Speaker->GetPNJQuest() != PRIEST_QUEST) | (step[PRIEST_QUEST] != 7))) | (PBook->GetFinalQuest() == true)) && ((Speaker->GetPNJQuest() != GUARD_QUEST) | (PBook->GetStatus(GUARD_QUEST) == 3)))
			step[Speaker->GetPNJQuest()]++;
		else
			hide();

	if ((step[Speaker->GetPNJQuest()] == 4) | ((Speaker->GetPNJQuest() == WARRIOR_QUEST) && (step[WARRIOR_QUEST] == 8)) | ((Speaker->GetPNJQuest() == PRIEST_QUEST) && (step[PRIEST_QUEST] == 9)) | ((Speaker->GetPNJQuest() == FINAL_QUEST) && (step[FINAL_QUEST] == 6)))
		hide();

	Refresh();
}

bool Dialogue::Run(Item *Cursor)
{
	if (QuestMsg.GetElementCount()) return Panel::Run(Cursor);

	return false;
}

bool Dialogue::Refresh()
{
	if ((Speaker == NULL) | (Listener == NULL))
		return false;

	if (Speaker->GetPNJQuest() == PRIEST_QUEST)
	{
		switch (PBook->GetStatus(PRIEST_QUEST))
		{
		case 1:
			break;
		case 2:
			if (step[PRIEST_QUEST] == 4)
				step[PRIEST_QUEST] = 5;
			if (PBook->GetPotionGift() >= TOTAL_POTION_GIFT)
			{
				Grid[1][1].display = true;
				Grid[2][1].display = false;
			}
			else
			{
				Grid[1][1].display = false;
				Grid[2][1].display = true;
			}
			break;
		case 3:
			Grid[1][1].display = Grid[2][1].display = true;
			break;
		}
	}
	else
		Grid[1][1].display = Grid[2][1].display = true;

	if (QuestMsg.ElementExist(step[Speaker->GetPNJQuest()]))
		((Text*)Grid[0][0].Content)->Update(320, RGB(0, 0, 0), RGB(255, 255, 0), QuestMsg.GetElement(step[Speaker->GetPNJQuest()])->getName());
	if (AnswerYesMsg.ElementExist(step[Speaker->GetPNJQuest()]))
		((Text*)Grid[1][0].Content)->Update(320, RGB(0, 0, 0), RGB(255, 255, 0), AnswerYesMsg.GetElement(step[Speaker->GetPNJQuest()])->getName());
	if (AnswerNoMsg.ElementExist(step[Speaker->GetPNJQuest()]))
		((Text*)Grid[2][0].Content)->Update(320, RGB(0, 0, 0), RGB(255, 255, 0), AnswerNoMsg.GetElement(step[Speaker->GetPNJQuest()])->getName());

	return Panel::Refresh();
}

bool Dialogue::generate(bool waited, Cursor *cursor)
{
	int i;

	if ((Speaker == NULL) | (Listener == NULL))
		return false;

	// Evénements bouton oui
	if (ClickOnGrid(1, 1, waited, cursor))
	{
		switch (Speaker->GetPNJQuest())
		{
		case WARRIOR_QUEST:
			switch (step[WARRIOR_QUEST])
			{
			case 2:
				PBook->SetStatus(WARRIOR_QUEST, 1);
				PBook->AddComment(PWorldTime->GetDay(), true, WARRIOR_STEP1, &EventsReporter);
				break;
			case 3:
				Speaker->Give(MagicCircle, Listener, &EventsReporter, &ActionReporter);
				Speaker->Give(MindProtector, Listener, &EventsReporter, &ActionReporter);
				PInventory->Refresh();
				break;
			case 5:
				Listener->SetNameID(WARRIOR_QUEST);
				Listener->GenerateName();
				PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
				PSave->SetRemainingPoints(Listener->GetSkillPoints(), Listener->GetMagicPoints());
				PSave->show();
				PSave->Refresh();
				PCharInfo->hide();
				PGauge->Refresh();
				break;
			case 7:
				if (PBook->GetFinalQuest())
				{
					Listener->AddToEquipment(Authorization, &EventsReporter, &ActionReporter);
					step[PRIEST_QUEST] = 9;
					PInventory->Refresh();
				}
				break;
			}
			break;
		case PRIEST_QUEST:
			switch (step[PRIEST_QUEST])
			{
			case 2:
				PBook->SetStatus(PRIEST_QUEST, 1);
				PBook->AddComment(PWorldTime->GetDay(), true, PRIEST_STEP1, &EventsReporter);
				break;
			case 5:
				for (i = 0; i < TOTAL_POTION_GIFT; i++)
					if (Listener->GetPotionGift()->GetElementCount())
					{
						Listener->Give(Potion::getPotionElement(0, Listener->GetPotionGift()), Speaker, &EventsReporter, &ActionReporter);
						PInventory->Refresh();
					}
				PBook->SetStatus(PRIEST_QUEST, 3);
				break;
			case 6:
				Speaker->Give(Daggy[0], Listener, &EventsReporter, &ActionReporter);
				Speaker->Give(MagicianDress[0], Listener, &EventsReporter, &ActionReporter);
				Listener->SetNameID(PRIEST_QUEST);
				Listener->GenerateName();
				PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
				PSave->SetRemainingPoints(Listener->GetSkillPoints(), Listener->GetMagicPoints());
				PSave->show();
				PSave->Refresh();
				PCharInfo->hide();
				PInventory->Refresh();
				PGauge->Refresh();
				break;
			case 8:
				if (PBook->GetFinalQuest())
				{
					Listener->AddToEquipment(Authorization, &EventsReporter, &ActionReporter);
					step[WARRIOR_QUEST] = 8;
					PInventory->Refresh();
				}
				break;
			}
			break;
		case GUARD_QUEST:
			if ((step[GUARD_QUEST] == 0) && (Listener->GetPocket(OTHERS)->SearchElement(Authorization) != -1))
				PBook->SetStatus(GUARD_QUEST, 3);
			break;
		case FINAL_QUEST:
			switch (step[FINAL_QUEST])
			{
			case 2:
				PBook->SetStatus(FINAL_QUEST, 1);
				PBook->AddComment(PWorldTime->GetDay(), true, FINAL_STEP1, &EventsReporter);
				break;
			case 3:
				for (i = 0; i < TOTAL_PICKUP_POTION_DF; i++)
					Speaker->Give(DiscretionFlow[i], Listener, &EventsReporter, &ActionReporter);
				PInventory->Refresh();
				break;
			case 5:
				Listener->SetNameID(FINAL_QUEST);
				Listener->GenerateName();
				PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
				PSave->SetRemainingPoints(Listener->GetSkillPoints(), Listener->GetMagicPoints());
				PSave->show();
				PSave->Refresh();
				PCharInfo->hide();
				PGauge->Refresh();
				break;
			}
			break;
		}

		NextStep();
	}

	// Evénements bouton non
	if (ClickOnGrid(1, 2, waited, cursor))
	{
		switch (Speaker->GetPNJQuest())
		{
		case WARRIOR_QUEST:
			if (step[WARRIOR_QUEST] == 4)
			{
				if (MagicCircle->GetOwnerID() == Listener->getId())
				{
					Listener->Give(MagicCircle, Speaker, &EventsReporter, &ActionReporter);
					PInventory->Refresh();
				}
				if (MindProtector->GetOwnerID() == Listener->getId())
				{
					Listener->Give(MindProtector, Speaker, &EventsReporter, &ActionReporter);
					PInventory->Refresh();
				}
			}
			if (step[WARRIOR_QUEST] == 3)
				step[WARRIOR_QUEST] = 4;
			else
				if (step[WARRIOR_QUEST] < 5)
				{
					step[WARRIOR_QUEST] = 0;
					PBook->SetStatus(WARRIOR_QUEST, 0);
				}
			break;
		case PRIEST_QUEST:
			if ((step[PRIEST_QUEST] != 5) && (step[PRIEST_QUEST] < 6))
			{
				step[PRIEST_QUEST] = 0;
				PBook->SetStatus(PRIEST_QUEST, 0);
			}
			break;
		case FINAL_QUEST:
			if (step[FINAL_QUEST] == 4)
				for (i = 0; i < TOTAL_PICKUP_POTION_DF; i++)
					if (DiscretionFlow[i]->GetOwnerID() == Listener->getId())
					{
						Listener->Give(DiscretionFlow[i], Speaker, &EventsReporter, &ActionReporter);
						PInventory->Refresh();
					}
			switch (step[FINAL_QUEST])
			{
			case 3:
				step[FINAL_QUEST] = 4;
				break;
			case 5:
				Listener->SetNameID(FINAL_QUEST);
				Listener->GenerateName();
				step[FINAL_QUEST] = 6;
				PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
				PSave->SetRemainingPoints(Listener->GetSkillPoints(), Listener->GetMagicPoints());
				PSave->show();
				PSave->Refresh();
				PCharInfo->hide();
				PGauge->Refresh();
				break;
			default:
				if (step[FINAL_QUEST] < 5)
				{
					step[FINAL_QUEST] = 0;
					PBook->SetStatus(FINAL_QUEST, 0);
				}
				break;
			}
			break;
		}
		hide();
	}

	return Panel::generate(waited, cursor);
}
