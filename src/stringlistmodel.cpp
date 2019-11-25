#include "stringlistmodel.h"
#include <QCoreApplication>
#include <iostream>
StringListModel::StringListModel(QListView *parent):QAbstractListModel(parent)
{
    //First real line should be at [1] for ease of use
    strList.push_back(new std::string);
    batchSize=this->initBatchSize;
}

int StringListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(strList.size());
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
    //int size=static_cast<int>(str.size());
    //this->beginInsertRows(QModelIndex(),rowCount(),rowCount()+size);
    strList.insert( strList.end(), str.begin(), str.end() );
    //this->endInsertRows();
    if(fetchedStrings<initBatchSize){
        fetchMore(QModelIndex());
    }
}

void StringListModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid())
        return;
    int remainder = static_cast<int>(strList.size()) - fetchedStrings;
    int itemsToFetch = qMin(batchSize, remainder);
    if (itemsToFetch <= 0)
        return;

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
    return (fetchedStrings < static_cast<int>(strList.size()));
}

int StringListModel::getFetchedStrings() const
{
    return fetchedStrings;
}


