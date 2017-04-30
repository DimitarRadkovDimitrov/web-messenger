#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "db.h"

#define MAX_QUERY 50000
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "dimitar"
#define PASSWORD  "0922289"
#define DATABASE  "dimitar"

int main(int argc, char *argv[])
{
	MYSQL mysql;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = NULL;

	mysql_init(&mysql);
	mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);
	char* query = malloc(sizeof(char) * MAX_QUERY);

	if (argv[1])
	{
		if (strcmp(argv[1], "-add") == 0)
		{
			initStreams(&mysql, argv[2], argv[3], query);
		}
		else if (strcmp(argv[1], "-remove") == 0)
		{
			removeUser(&mysql, argv[2], argv[3], query);
		}
		else if (strcmp(argv[1], "-post") == 0)
		{
			post(&mysql, argv[2], argv[3], argv[4], query);
		}
		else if (strcmp(argv[1], "-find") == 0)
		{
			findStreams(&mysql, &res, row, argv[2], query);
		}
		else if (strcmp(argv[1], "-view") == 0)
		{
			view(&mysql, &res, row, argv[2], argv[3], argv[4], argv[5], query);
		}
	}

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-clear") == 0)
		{
			clearAll(&mysql, &res, row, query);
		}
		else if (strcmp(argv[i], "-reset") == 0)
		{
			dropAll(&mysql, &res, row, query);
		}
		else if (strcmp(argv[i], "-posts") == 0)
		{
			showPosts(&mysql, &res, row, query);
		}
		else if (strcmp(argv[i], "-streams") == 0)
		{
			showStreams(&mysql, &res, row, query);
		}
		else if (strcmp(argv[i], "-users") == 0)
		{
			showUsers(&mysql, &res, row, query);
		}
		else if (strcmp(argv[i], "-help") == 0)
		{
			printf("Run ./db with arguments/flags:\n");
			printf("------------------------------\n");
			printf("-clear -> to wipe existing tables in the database\n");
			printf("-reset -> to remove all existing tables in the database\n");
			printf("-posts -> to display all the stream posts in the database\n");
			printf("-users -> to display all the users in the database\n");
			printf("-streams -> to display all the stream names in the database\n");
		}
	}

	mysql_close(&mysql);
	mysql_library_end();
	free(query);
	return 0;
}

void initStreams(MYSQL* mysql, char* username, char* stream, char* query)
{
	query[0] = '\0';

	strcat(query, "create table if not exists Streams (");
	strcat(query, "streamId int primary key auto_increment,");
	strcat(query, "streamName varchar(50) not null unique);");

	mysql_query(mysql, query);

	query[0] = '\0';

	strcat(query, "create table if not exists Users (");
	strcat(query, "userId int primary key auto_increment,");
	strcat(query, "userName varchar(50) not null,");
	strcat(query, "streamName varchar(50) not null,");
	strcat(query, "postsRead int default 0,");
	strcat(query, "constraint myConstraint unique(userName,streamName));");

	mysql_query(mysql, query);

	query[0] = '\0';

	strcat(query, "insert into Users ");
	strcat(query, "(userName, streamName) ");
	strcat(query, "values ('");
	strcat(query, username);
	strcat(query, "', '");
	strcat(query, stream);
	strcat(query, "');");

	mysql_query(mysql, query);
	query[0] = '\0';

	strcat(query, "create table if not exists ");
	strcat(query, stream);
	strcat(query, "(streamId int primary key auto_increment,");
	strcat(query, "sender varchar(50) not null,");
	strcat(query, "message text);");

	mysql_query(mysql, query);
	query[0] = '\0';

	strcat(query, "insert into Streams ");
	strcat(query, "(streamName) ");
	strcat(query, "values ('");
	strcat(query, stream);
	strcat(query, "');");

	mysql_query(mysql, query);
	query[0] = '\0';
}

void removeUser(MYSQL* mysql, char* username, char* stream, char* query)
{
	query[0] = '\0';

	strcat(query, "delete from ");
	strcat(query, "Users where ");
	strcat(query, "Users.userName = '");
	strcat(query, username);
	strcat(query, "' and Users.streamName = '");
	strcat(query, stream);
	strcat(query, "';");

	mysql_query(mysql, query);
	query[0] = '\0';
}

void post(MYSQL* mysql, char* username, char* stream, char* message, char* query)
{
	query[0] = '\0';

	strcat(query, "insert into ");
	strcat(query, stream);
	strcat(query, "(message,sender)");
	strcat(query, " values ('");
	strcat(query, message);
	strcat(query, "','");
	strcat(query, username);
	strcat(query, "');");

	mysql_query(mysql, query);
	query[0] = '\0';
}

void findStreams(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* username, char* query)
{
	query[0] = '\0';

	strcat(query, "select Users.streamName ");
	strcat(query, "from Users where Users.userName='");
	strcat(query, username);
	strcat(query, "';");

	mysql_query(mysql, query);
	(*res) = mysql_store_result(mysql);

	while ((row = mysql_fetch_row(*res))) {
		for (int i = 0; i < mysql_num_fields(*res); i++)
		{
			printf("%s", row[i]);
		}
		printf("\n");
	}

	mysql_free_result((*res));
	query[0] = '\0';
}

void view(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* username, char* stream, char* index, char* flag, char* query)
{
	int totalPosts = 0;
	int currPost = 0;
	int postsRead = 0;
	char temp[15];

	query[0] = '\0';

	if (strcmp(stream, "all") == 0)
	{
		strcat(query, "create table if not exists allStream ");
		strcat(query, "(streamId int primary key auto_increment,");
		strcat(query, "sender varchar(50) not null,");
		strcat(query, "message text);");

		mysql_query(mysql, query);
		query[0] = '\0';

		strcat(query, "select Users.streamName ");
		strcat(query, "from Users where Users.userName='");
		strcat(query, username);
		strcat(query, "';");

		mysql_query(mysql, query);
		query[0] = '\0';
		(*res) = mysql_store_result(mysql);

		while ((row = mysql_fetch_row(*res)))
		{
			strcat(query, "insert into allStream (message,sender) ");
			strcat(query, "select message,sender from ");
			strcat(query, row[0]);
			strcat(query, "; ");

			mysql_query(mysql, query);
			query[0] = '\0';
		}
		mysql_free_result((*res));
	}

	strcat(query, "select count(*) from ");
	strcat(query, stream);

	if (strcmp(stream, "all") == 0)
	{
		strcat(query, "Stream");
	}

	mysql_query(mysql, query);
	(*res) = mysql_store_result(mysql);

	totalPosts = atoi(*mysql_fetch_row(*res));
	mysql_free_result((*res));

	query[0] = '\0';

	if (index)
	{
		currPost = atoi(index);

		if (flag)
		{
			if ((strcmp(flag, "-p") == 0) && (currPost - 1 >= 0))
			{
				currPost = currPost - 1;
			}
			else if (strcmp(flag, "-m") == 0)
			{
				if (strcmp(stream, "all") != 0)
				{
					currPost = totalPosts - 1;

					sprintf(temp, "%d", totalPosts);
					strcat(query, "update Users set postsRead=");
					strcat(query, temp);
					strcat(query, " where userName='");
					strcat(query, username);
					strcat(query, "' and streamName='");
					strcat(query, stream);
					strcat(query, "';");

					mysql_query(mysql, query);

					temp[0] = '\0';
					query[0] = '\0';
				}
				else
				{
					currPost = 1;
				}
			}
			else if ((strcmp(flag, "-n") == 0) && (currPost + 1 < totalPosts))
			{
				currPost = currPost + 1;

				if (strcmp(stream, "all") != 0)
				{
					if (currPost + 1 > postsRead)
					{
						sprintf(temp, "%d", currPost + 1);
						strcat(query, "update Users set postsRead=");
						strcat(query, temp);
						strcat(query, " where userName='");
						strcat(query, username);
						strcat(query, "' and streamName='");
						strcat(query, stream);
						strcat(query, "';");

						mysql_query(mysql, query);

						temp[0] = '\0';
						query[0] = '\0';
					}
				}
			}

			sprintf(temp, "%d", currPost);

			query[0] = '\0';

			strcat(query, "select message from ");
			strcat(query, stream);

			if (strcmp(stream, "all") == 0)
			{
				strcat(query, "Stream");
			}

			strcat(query, " limit ");
			strcat(query, temp);
			strcat(query, ",1;");

			mysql_query(mysql, query);
			(*res) = mysql_store_result(mysql);

			printf("%d\n", currPost);
			printf("%s", *mysql_fetch_row(*res));
			mysql_free_result((*res));

			temp[0] = '\0';
			query[0] = '\0';
		}
	}
	else
	{
		if (strcmp(stream, "all") != 0)
		{
			strcat(query, "select Users.postsRead from Users ");
			strcat(query, "where Users.userName='");
			strcat(query, username);
			strcat(query, "' and Users.streamName='");
			strcat(query, stream);
			strcat(query, "';");

			mysql_query(mysql, query);
			query[0] = '\0';

			(*res) = mysql_store_result(mysql);
			postsRead = atoi(*mysql_fetch_row(*res));
			mysql_free_result((*res));

			if (postsRead == totalPosts)
			{
				currPost = postsRead;
				currPost = currPost - 1;
			}
			else
			{
				currPost = postsRead;
			}
		}
		else
		{
			currPost = 0;
		}

		sprintf(temp, "%d", currPost);

		strcat(query, "select message from ");
		strcat(query, stream);

		if (strcmp(stream, "all") == 0)
		{
			strcat(query, "Stream");
		}
		strcat(query, " limit ");
		strcat(query, temp);
		strcat(query, ",1;");

		temp[0] = '\0';

		mysql_query(mysql, query);
		(*res) = mysql_store_result(mysql);

		printf("%d\n", currPost);
		printf("%s", *mysql_fetch_row(*res));
		mysql_free_result((*res));

		query[0] = '\0';

		if ((currPost + 1 > postsRead) && (strcmp(stream, "all") != 0))
		{
			sprintf(temp, "%d", currPost + 1);
			strcat(query, "update Users set postsRead=");
			strcat(query, temp);
			strcat(query, " where userName='");
			strcat(query, username);
			strcat(query, "' and streamName='");
			strcat(query, stream);
			strcat(query, "';");

			mysql_query(mysql, query);

			temp[0] = '\0';
			query[0] = '\0';
		}
	}

	strcpy(query, "truncate allStream;");
	mysql_query(mysql, query);
	query[0] = '\0';
}

void dropAll(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query)
{
	query[0] = '\0';

	strcat(query, "select streamName from Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';

	(*res) = mysql_store_result(mysql);

	if (*res)
	{
		while ((row = mysql_fetch_row(*res)))
		{
			for (int i = 0; i < mysql_num_fields(*res); i++)
			{
				strcpy(query, "drop table ");
				strcat(query, row[i]);
				strcat(query, ";");
				mysql_query(mysql, query);
			}
		}
	}

	mysql_free_result((*res));
	query[0] = '\0';

	strcpy(query, "drop table Users;");
	mysql_query(mysql, query);

	query[0] = '\0';

	strcpy(query, "drop table Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';

	strcpy(query, "drop table allStream;");
	mysql_query(mysql, query);

	query[0] = '\0';
}

void clearAll(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query)
{
	query[0] = '\0';

	strcat(query, "select streamName from Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';

	(*res) = mysql_store_result(mysql);

	if (*res)
	{
		while ((row = mysql_fetch_row(*res))) {
			for (int i = 0; i < mysql_num_fields(*res); i++)
			{
				strcpy(query, "truncate ");
				strcat(query, row[i]);
				strcat(query, ";");
				mysql_query(mysql, query);
			}
		}
	}

	mysql_free_result((*res));

	query[0] = '\0';

	strcpy(query, "truncate allStream;");
	mysql_query(mysql, query);

	query[0] = '\0';

	strcpy(query, "truncate Users;");
	mysql_query(mysql, query);

	query[0] = '\0';

	strcpy(query, "truncate Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';
}

void showStreams(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query)
{
	query[0] = '\0';

	strcat(query, "select streamName from Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';

	(*res) = mysql_store_result(mysql);

	if (*res)
	{
		while ((row = mysql_fetch_row(*res))) {
			for (int i = 0; i < mysql_num_fields(*res); i++)
			{
				printf("%s\n", row[i]);
			}
		}
	}

	mysql_free_result((*res));
	query[0] = '\0';
}

void showUsers(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query)
{
	query[0] = '\0';

	strcat(query, "select distinct userName from Users;");
	mysql_query(mysql, query);

	query[0] = '\0';

	(*res) = mysql_store_result(mysql);

	if (*res)
	{
		while ((row = mysql_fetch_row(*res))) {
			for (int i = 0; i < mysql_num_fields(*res); i++)
			{
				printf("%s\n", row[i]);
			}
		}
	}

	mysql_free_result((*res));
	query[0] = '\0';
}

void showPosts(MYSQL* mysql, MYSQL_RES** res, MYSQL_ROW row, char* query)
{
	MYSQL_RES *message = NULL;
	MYSQL_ROW post = NULL;
	query[0] = '\0';

	strcat(query, "select streamName from Streams;");
	mysql_query(mysql, query);

	query[0] = '\0';

	(*res) = mysql_store_result(mysql);

	if (*res)
	{
		while ((row = mysql_fetch_row(*res)))
		{
			for (int i = 0; i < mysql_num_fields(*res); i++)
			{
				strcat(query, "select message from ");
				strcat(query, row[i]);
				strcat(query, "; ");

				mysql_query(mysql, query);
				message = mysql_store_result(mysql);
				query[0] = '\0';

				while ((post = mysql_fetch_row(message)))
				{
					for (int i = 0; i < mysql_num_fields(message); i++)
					{
						printf("%s\n", post[i]);
					}
				}

				mysql_free_result(message);

			}
		}
	}

	mysql_free_result((*res));
}
