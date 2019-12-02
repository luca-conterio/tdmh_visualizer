#include "stringlistmodel.h"
#include <QCoreApplication>
#include <iostream>
StringListModel::StringListModel(QListView *parent):QAbstractListModel(parent),listParent(parent)
{
    //First real line should be at [1] for ease of use
    strList.push_back(new std::string);
    batchSize=this->initBatchSize;
}

int StringListModel::rowCount(const QModelIndex& /*parent*/) const
{
    //return static_cast<int>(strList.size());
    return fetchedStrings;
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() > static_cast<int>(strList.size()))
        return QVariant();
    if(role==Qt::DisplayRole){
        auto rowN=static_cast<unsigned long>(index.row());
        return QString::fromStdString( /*std::to_string(index.row())+*/
                    (* (strList[rowN]))
                    );
    }



    return QVariant();
}

void StringListModel::addString(std::vector<std::string*> str)
{
    strList.insert( strList.end(), str.begin(), str.end() );

    if(realt ||fetchedStrings<initBatchSize){
        fetchMore(QModelIndex());
        this->listParent->repaint();
    }
}

void StringListModel::fetchMore(const QModelIndex &parent)
{
    //If parent already valid or batch batch is disabled and not in realtime yet
    if (parent.isValid() || (batchUpdateDisabled&&!realt))
        return;
    int remainder = static_cast<int>(strList.size()) - fetchedStrings;
    int itemsToFetch = qMin(batchSize, remainder);
    if (itemsToFetch <= 0)
        return;

    itemsToFetch = (batchUpdateDisabled)?remainder:itemsToFetch;
    beginInsertRows(QModelIndex(), fetchedStrings, fetchedStrings + itemsToFetch - 1);
    fetchedStrings += itemsToFetch;
    endInsertRows();
    this->batchSize=this->rowCount()/batchIncreaseDivisor; //Load % of size to scroll fast on long logs
    emit this->fetched();
}

bool StringListModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid())
        return false;
    //If batch not disabled or in real time can consider reading
    return (!batchUpdateDisabled||realt) && (fetchedStrings < static_cast<int>(strList.size()));
}

int StringListModel::getFetchedStrings() const
{
    return fetchedStrings;
}

void StringListModel::goRealTime()
{
    this->realt=true;
    if(this->batchUpdateDisabled){
        fetchMore(QModelIndex());
    }
}

void StringListModel::disableBatchUpdates(){
    this->batchUpdateDisabled=true;
}

