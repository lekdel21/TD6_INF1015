// Le Modèle pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Caisse.hpp"
#include <iostream>

Caisse::Caisse() : totalSansTax_(0)
{
	articles_ = {};
}

Caisse::~Caisse()
{
	while (!articles_.empty())
	{
		delete articles_.back();
		articles_.back() = nullptr;
		articles_.pop_back();
	}
}


// slots
void Caisse::addArticle(Article* article)
{
	if (article->description == "" || article->prix == 0)
	{
		throw invalid_argument("Les informations sur l'article sont incomplets.");
	}
	else
	{
		articles_.push_back(article);
		emit articleAdded(article);
	}
}

void Caisse::delArticle(Article* article)
{
	auto it = find(articles_.begin(), articles_.end(), article);
	if (it != articles_.end()) {
		Article* a = *it;
		articles_.erase(it);
		emit articleDeleted(a);
	}
}

double Caisse::totalTaxs(const function<double(double)>& critere)
{
	int i = 0;
	double total = 0;
	while (articles_[i] != articles_.back())
	{
		if (articles_[i]->taxable)
		{
			total += critere(articles_[i]->prix);
		}
		i++;
	}
	return total;
}