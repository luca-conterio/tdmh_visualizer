#ifndef STRINGLISTMODEL_H
#define STRINGLISTMODEL_H

#include <QAbstractListModel>
#include <QListView>

/*!
 * \brief The StringListModel class implements a list model based on strings rather than QStrings, in order to save memory. Fetching is done on demand to avoid ui freezes
 * \author Francesco Franzini
 */
class StringListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    /*!
     * \brief StringListModel initializes the model and adds a string to start line numbers from 1
     * \param parent parent view
     */
    StringListModel(QListView *parent=nullptr);

    /*!
     * \brief rowCount returns the row count of inserted(not necessarily in view yet) lines
     * \param parent parent index
     * \return row count
     */
    int rowCount(const QModelIndex &parent= QModelIndex()) const override;

    /*!
     * \brief data fetches a specific line and transforms it into a QString
     * \param index index to fetch
     * \param role returns empty invariant if different from Qt::DisplayRole
     * \return the QString representation of the line
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    /*!
     * \brief fetchMore if there are lines not inserted in the view, inserts them in batches of incremental size
     * \param parent parent index
     */
    void fetchMore(const QModelIndex &parent) override;

    /*!
     * \brief canFetchMore checks if there are lines in storage that are not yet in view
     * \param parent parent index
     * \return view lines < storage lines
     */
    bool canFetchMore(const QModelIndex &parent) const override;

    /*!
     * \brief getFetchedStrings returns the number of rows fetched and put in view
     * \return number of view-processed lines
     */
    int getFetchedStrings() const;

    /*!
     * \brief goRealTime puts this model in real time mode, adding rows to view as soon as they are inserted
     */
    void goRealTime();

    /*!
     * \brief disableBatchUpdates disables view updates until goRealTime() is called
     */
    void disableBatchUpdates();

private:
    const int initBatchSize=1000;
    const int batchIncreaseDivisor=10;

    bool realt=false;
    bool batchUpdateDisabled=false;

    int fetchedStrings=1;
    std::vector<std::string*> strList;

    int batchSize;
    QListView * const listParent;

public slots:
    /*!
     * \brief addString appends a vector of lines to the storage, without updating the view
     * \param str the vector of strings
     */
    void addString(const std::vector<std::string *> str);



signals:

    /*!
     * \brief fetched emitted after a fetcMore call completes loading rows in view
     */
    void fetched();
};

#endif // STRINGLISTMODEL_H
