#ifndef BOT_TABLE_VIEW_H
#define BOT_TABLE_VIEW_H

#include <QAbstractTableModel>
#include <QString>
#include <QDateTime>
#include <vector>

struct BotDto
{
	uint32_t Id;
	QString Host;
	QString Username;
	QString ExternalIp;
	QString InternalIp;
	QString Architecture;
	QString OsVersion;
	QString LastPingUtc;
};

class BotTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	BotTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent), fetchedRowCount(0), batchSize(20)
	{

	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (parent.isValid())
			return 0;
		return fetchedRowCount;
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return 8; // Number of properties in BotDto
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (!index.isValid() || role != Qt::DisplayRole)
			return QVariant();

		if (index.row() >= fetchedRowCount)
			return QVariant();

		const BotDto& bot = bots.at(index.row());

		switch (index.column())
		{
		case 0: return bot.Id;
		case 1: return bot.Host;
		case 2: return bot.Username;
		case 3: return bot.ExternalIp;
		case 4: return bot.InternalIp;
		case 5: return bot.Architecture;
		case 6: return bot.OsVersion;
		case 7: return bot.LastPingUtc;
		default: return QVariant();
		}
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
	{
		if (role != Qt::DisplayRole)
			return QVariant();

		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case 0: return "ID";
			case 1: return "Host";
			case 2: return "Username";
			case 3: return "External IP";
			case 4: return "Internal IP";
			case 5: return "Architecture";
			case 6: return "OS Version";
			case 7: return "Last Ping UTC";
			default: return QVariant();
			}
		}
		return QVariant();
	}

	void addBots(const std::vector<BotDto>& newBots)
	{
		beginResetModel();
		bots = newBots;
		fetchedRowCount = std::min(batchSize, static_cast<int>(bots.size()));
		endResetModel();
	}

	void addBot(const BotDto newBot)
	{
		// Check if the maximum number of fetched rows has been reached
		if (fetchedRowCount >= batchSize) {
			// If so, we need to inform the model that more data is available before adding the new bot
			emit canFetchMore(QModelIndex());
			return;
		}

		// Add the new bot to the internal vector
		beginInsertRows(QModelIndex(), fetchedRowCount, fetchedRowCount);
		bots.push_back(newBot);
		fetchedRowCount++;
		endInsertRows();
	}

	bool canFetchMore(const QModelIndex& parent) const override
	{
		if (parent.isValid())
			return false;
		return fetchedRowCount < bots.size();
	}

	void fetchMore(const QModelIndex& parent) override
	{
		if (parent.isValid())
			return;

		int remainder = bots.size() - fetchedRowCount;
		int itemsToFetch = std::min(batchSize, remainder);

		if (itemsToFetch <= 0)
			return;

		beginInsertRows(QModelIndex(), fetchedRowCount, fetchedRowCount + itemsToFetch - 1);
		fetchedRowCount += itemsToFetch;
		endInsertRows();
	}

	void updateBot(const BotDto& bot)
	{
		for (int row = 0; row < bots.size(); ++row)
		{
			if (bots[row].Id == bot.Id)
			{
				bots[row] = bot;

				emit dataChanged(index(row, 0), index(row, columnCount() - 1));
				break;
			}
		}
	}

	void deleteBot(int id)
	{
		for (int row = 0; row < bots.size(); ++row)
		{
			if (bots[row].Id == id)
			{
				beginRemoveRows(QModelIndex(), row, row);
				bots.erase(bots.begin() + row);
				fetchedRowCount = std::min(fetchedRowCount, static_cast<int>(bots.size()));
				endRemoveRows();
				break;
			}
		}
	}

	void clear()
	{
		beginResetModel();
		bots.clear();
		fetchedRowCount = 0;
		endResetModel();
	}

private:
	std::vector<BotDto> bots;
	int fetchedRowCount;
	const int batchSize;
};

#endif // !BOT_TABLE_H
