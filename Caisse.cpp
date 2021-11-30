﻿// Le Modèle pour calculatrice simple.
// Par Francois-R.Boyer@PolyMtl.ca

#include "Caisse.hpp"
#include <iostream>

Caisse::Caisse() : totalSansTax_(0)
{
	articles_ = {};
}


// slots
void Caisse::addArticle(Article* article)
{
	articles_.push_back(article); 
	emit articleAdded(article);
}

void Caisse::delArticle(Article* article)
{
	auto it = find(articles_.begin(), articles_.end(), article);
	if (it != articles_.end()) {
		Article* a = *it;
		articles_.erase(it);
	}
	emit articleDeleted(article);
}