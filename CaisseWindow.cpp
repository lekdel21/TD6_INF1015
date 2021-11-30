// La Vue-Controlleur pour caisse enregistreuse simple.
// Par Alek Delisle et Julion Malek.

#include "CaisseWindow.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QcheckBox>
#include <QListWidget>
#include <QVariant>
#include <Qstring>
#pragma pop()
#include <iostream>
#include <type_traits>
#include <cppitertools/range.hpp>

#include <iostream>

using iter::range;

CaisseWindow::CaisseWindow(QWidget* parent) :
	QMainWindow(parent)
{
	caisse_ = new Caisse();
	setup();

}
CaisseWindow::~CaisseWindow() {};


// Ajout d'article
void CaisseWindow::addArticle()
{
	Article* newArticle = new Article;
	newArticle->description = descriptionEditor_->text().toStdString();
	newArticle->prix = prixEditor_->text().toDouble();
	newArticle->taxable = taxEditor_->isChecked();
	caisse_->addArticle(newArticle);
	caisse_->addToTotal(newArticle->prix);
}

void CaisseWindow::articleHasBeenAdded(Article* article)
{
	int taillePrix = to_string(article->prix).size() - 4;
	string affichagePrix = to_string(round(article->prix * 100) / 100).erase(taillePrix, 4);

	string infoArticle = article->description + "\t" + affichagePrix + "$" + "\t" + to_string(article->taxable);
	QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(infoArticle), articlesList_);
	item->setData(Qt::UserRole, QVariant::fromValue<Article*>(article));

	// Decalage------------------------------------------------------------------- + REPETITION
	int tailleTotal = to_string(caisse_->getTotal()).size() - 4;
	prixBTaxLabel_->setText((to_string(round(caisse_->getTotal() * 100) / 100).erase(tailleTotal, 4) + "$").c_str());
}

// Retirer article
void CaisseWindow::delArticle()
{
	vector<Article*> toDelete;
	for (QListWidgetItem* item : articlesList_->selectedItems()) {
		toDelete.push_back(item->data(Qt::UserRole).value<Article*>());
	}
	for (Article* a : toDelete) {
		caisse_->delArticle(a);
		caisse_->removeToTotal(a->prix);
	}

	// Decalage------------------------------------------------------------------- + REPETITION
	int tailleTotal = to_string(caisse_->getTotal()).size() - 4;
	prixBTaxLabel_->setText((to_string(round(caisse_->getTotal() * 100) / 100).erase(tailleTotal, 4) + "$").c_str());
}

void CaisseWindow::articleHasBeenDeleted(Article* article)
{
	for (int i = 0; i < articlesList_->count(); ++i) {
		QListWidgetItem* item = articlesList_->item(i);
		Article* a = item->data(Qt::UserRole).value<Article*>();
		if (a == article) {
			delete item;
			break;
		}
	}
}

// Renialiser les articles
void CaisseWindow::reniArticles()
{
	vector < Article* > toDelete;
	for (int i = 0; i < articlesList_->count(); ++i) {
		toDelete.push_back(articlesList_->item(i)->data(Qt::UserRole).value<Article*>());
	}
	for (Article* a : toDelete) {
		caisse_->delArticle(a);
	}
}

//Affichage
void CaisseWindow::setUI()
{

	//	Champs pour la description.
	QLabel* descriptionLabel = new QLabel;
	descriptionLabel->setText("Article:");
	descriptionEditor_ = new QLineEdit;
	
	QHBoxLayout* descriptionLayout = new QHBoxLayout;
	descriptionLayout->addWidget(descriptionLabel);
	descriptionLayout->addWidget(descriptionEditor_);

	//	Champs pour le prix.
	QLabel* prixLabel = new QLabel;
	prixLabel->setText("Prix:");
	prixEditor_ = new QLineEdit;

	QBoxLayout* prixLayout = new QHBoxLayout(this);
	prixLayout->addWidget(prixLabel);
	prixLayout->addWidget(prixEditor_);

	// Bouton pour ajouter un article.
	QPushButton* addArticleButton = new QPushButton;
	addArticleButton->setText("Ajouter");
	connect(addArticleButton, SIGNAL(clicked()), this, SLOT(addArticle()));

	//	Bouton pour retirer un article.
	QPushButton* delArticleButton = new QPushButton(this);
	delArticleButton->setText("Retirer");
	connect(delArticleButton, SIGNAL(clicked()), this, SLOT(delArticle()));

	//	Bouton pour reinitiliser.
	QPushButton* renitializeButton = new QPushButton(this);
	renitializeButton->setText("Renitialiser");
	connect(renitializeButton, SIGNAL(clicked()), this, SLOT(reniArticles()));

	//	CheckBox pour indiquer si l'article est taxable
	QLabel* taxLabel = new QLabel;
	taxLabel->setText("tax");
	taxEditor_ = new QCheckBox(this);

	QBoxLayout* taxLayout = new QHBoxLayout(this);
	prixLayout->addWidget(taxLabel);
	prixLayout->addWidget(taxEditor_);

	// On place tout dans la boite vertical.
	QVBoxLayout* menuLayout = new QVBoxLayout;
	menuLayout->addLayout(descriptionLayout);
	menuLayout->addLayout(prixLayout);
	menuLayout->addWidget(addArticleButton);
	menuLayout->addWidget(delArticleButton);
	menuLayout->addWidget(renitializeButton);
	menuLayout->addLayout(taxLayout);

	// ------------------------ Deuxieme partie du layout -----------------------------
	//	Affichage des articles.
	QLabel* descriptListLabel = new QLabel;
	descriptListLabel->setText("Description \t prix \t taxable");
	articlesList_ = new QListWidget(this);

	//	Affichage des totaux
	prixBTaxLabel_ = new QLabel;
	int tailleTotal = to_string(caisse_->getTotal()).size() - 4;
	prixBTaxLabel_->setText((to_string(round(caisse_->getTotal() * 100) / 100).erase(tailleTotal, 4) + "$").c_str());

	QLabel* sousTotalLabel = new QLabel;
	sousTotalLabel->setText("Sous-Total: ");


	QHBoxLayout* sousTotalLayout = new QHBoxLayout;
	sousTotalLayout->addWidget(sousTotalLabel);
	sousTotalLayout->addWidget(prixBTaxLabel_);


	QLabel* totalTaxLabel = new QLabel;
	totalTaxLabel->setText("Tax: ");

	QLabel* prixATaxLabel = new QLabel;
	prixATaxLabel->setText("Total: ");


	//	 On place tout dans la deuxieme boite verticale.
	QVBoxLayout* listLayout = new QVBoxLayout(this);
	listLayout->addWidget(descriptListLabel);
	listLayout->addWidget(articlesList_);
	listLayout->addLayout(sousTotalLayout);
	listLayout->addWidget(totalTaxLabel);
	listLayout->addWidget(prixATaxLabel);

	//	Ligne de separation.
	QFrame* verticalFrameLine = new QFrame;
	verticalFrameLine->setFrameShape(QFrame::VLine);

	//	Configuration de la fenetre au complet.
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(menuLayout);
	mainLayout->addWidget(verticalFrameLine);
	mainLayout->addLayout(listLayout);

	//	
	QWidget* widget = new QWidget;
	widget->setLayout(mainLayout);


	setCentralWidget(widget);

	string title = "Caisse enregistreuse";
	setWindowTitle(title.c_str());
}
void CaisseWindow::setup()
{
	setUI();

	connect(caisse_, SIGNAL(articleAdded(Article*)), this, SLOT(articleHasBeenAdded(Article*)));
	connect(caisse_, SIGNAL(articleDeleted(Article*)), this, SLOT(articleHasBeenDeleted(Article*)));
}

