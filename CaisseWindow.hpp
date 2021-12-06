#pragma once
// La Vue-Controlleur pour la caisse enregistreuse.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Caisse.hpp"
#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QMainWindow>
#include <QPushButton>
#include <QlineEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QLabel>
#include <memory>
#pragma pop()

class CaisseWindow : public QMainWindow 
{
	Q_OBJECT

public:
	CaisseWindow(QWidget* parent = nullptr);
	~CaisseWindow();

public slots:
	void addArticle();
	void delArticle();
	void reniArticles();
	void modifyTotaux();

	void articleHasBeenAdded(Article* article);
	void articleHasBeenDeleted(Article* article);

private:
	void setup();
	void setUI();

	Caisse* caisse_;

	QListWidget* articlesList_;
	QLineEdit* descriptionEditor_, * prixEditor_;
	QCheckBox* taxEditor_;
	QLabel *prixBTaxLabel_, *prixTaxLabel_;
	QPushButton* delArticleButton_;
};
