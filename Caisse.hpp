#pragma once
// Le Modèle pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#pragma warning(push, 0) // Sinon Qt fait des avertissements à /W4.
#include <QObject>
#pragma pop()
#include <functional>
#include <vector>

using namespace std;

struct Article {
	string description;
	double prix;
	bool taxable;
};

class Caisse : public QObject 
{
	Q_OBJECT
public:
	Caisse();
	~Caisse();
	double getTotal() { return totalSansTax_; }
	void addToTotal(double ajout) { totalSansTax_ += ajout; }
	void removeToTotal(double enleve) { totalSansTax_ -= enleve; }
	double totalTaxs();

public slots:
	void addArticle(Article* article);
	void delArticle(Article* article);

signals:
	void articleAdded(Article* article);
	void articleDeleted(Article* article);

private:
	vector<Article*> articles_;
	double totalSansTax_;

};
