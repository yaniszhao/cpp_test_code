#include <stdio.h>

#include <sqlite3.h>

int main()
{
	sqlite3 *db = NULL;
	if(sqlite3_open("my.db", &db)){
		printf("sqlite3_open fail: %s\n", sqlite3_errmsg(db));
		return -1;
	}

#if 1
	char *errmsg = NULL;
	if(sqlite3_exec(db, \
		"CREATE TABLE demo (account TEXT, passwd TEXT)",\
		NULL, NULL, &errmsg)){
		//printf("CREATE TABLE demo err: %s\n", errmsg);
		printf("CREATE TABLE demo fail: %s\n", sqlite3_errmsg(db));
		return -1;
	};

	if(sqlite3_exec(db, \
		"INSERT INTO demo VALUES(\"jane\", \"123456789\")",\
		NULL, NULL, &errmsg)){
		printf("CREATE TABLE demo err: %s\n", errmsg);
		printf("CREATE TABLE demo fail: %s\n", sqlite3_errmsg(db));
		return -1;
	};

	if(sqlite3_exec(db, \
		"INSERT INTO demo VALUES(\"jack\", \"1qw2eqwrwq\")",\
		NULL, NULL, &errmsg)){
		printf("CREATE TABLE demo err: %s\n", errmsg);
		printf("CREATE TABLE demo fail: %s\n", sqlite3_errmsg(db));
		return -1;
	};

#endif
	sqlite3_close(db);
}

/* ==================================================== */

#include <stdio.h>

#include <sqlite3.h>

/*
 *void *para - 参数
 *f_num - 一行内容的总数量(列数)
 *char **f_val - 得到结果的首地址
	|----------|
	| jane     |
	| 12345678 |
	....
 *char **f_name
		|----------|
		| account  |
		| passwd   |
 */
int callback(void *para, int f_num, char **f_val, char **f_name)
{
	int i;
	printf("===%s===%s===%s===\n",(char *)para, f_name[0], f_name[1]);
	for (i=0; i < f_num; i+=2){
		printf("%-8s : %-8s\n", f_val[i], f_val[i+1]);
	}

	return 0;
}


int main()
{
	sqlite3 *db = NULL;
	if(sqlite3_open("my.db", &db)){
		printf("sqlite3_open fail: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	char *errmsg = NULL;

 	char **result;
	int nrow, ncolumn, index;

	/*
	 *db - 句柄
	 *"select * from demo" - sql语句（查找表单demo中的所有内容）
	 *result - 得到结果的首地址
		|----------|
		| account  |
		| passwd   |
		| jane     |
		| 12345678 |
		....
	 *nrow - 行数
	 *ncolumn- 列数
	 */
	if(sqlite3_get_table(db, "select * from demo", &result, &nrow, &ncolumn, &errmsg)){
		printf("error : %s\n", sqlite3_errmsg(db));
		return -1;
	}

	/*得到正文的id*/
	index = ncolumn;
	while(result[index]){
		printf("%-8s : %-8s\n", result[index], result[index+1]);
		index += 2;
	}

	/*
	 *db - 句柄
	 *"select * from demo" - sql语句（查找表单demo中的所有内容）
	 *callback - 得到结果的回调函数
	 */
	if (sqlite3_exec(db, "select * from demo", callback, "hello world", &errmsg) != 0){
		printf("error : %s\n", sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_free_table(result);
	sqlite3_close(db);
}

/* ================================================== */

#include <stdio.h>

#include <pthread.h>
#include <sqlite3.h>

struct dbarg{
	sqlite3 *db;
	char *account;
	char *passwd;
};

#define MAX 64

void *do_datebase(void *arg)
{
	struct dbarg *p = (struct dbarg *)arg;
	char *errmsg = NULL;

	char sqlcmd[MAX];
	snprintf(sqlcmd, MAX, \
		 "INSERT INTO demo VALUES(\"%s\", \"%s\")", \
		 p->account, p->passwd);

	
	printf("CMD:%s\n", sqlcmd);
	sleep(1);

	if(sqlite3_exec(p->db, \
		sqlcmd,\
		NULL, NULL, &errmsg)){
		printf("INSERT INTO demo VALUES: %s\n", sqlite3_errmsg(p->db));
		return (void *)-1;
	};

	printf("INSERT msg done.\n");

	return (void *)0;
}

int main()
{
	char *errmsg = NULL;
	sqlite3 *db1 = NULL;
	if(sqlite3_open("my.db", &db1)){
		printf("sqlite3_open fail: %s\n", sqlite3_errmsg(db1));
		return -1;
	}
	if(sqlite3_exec(db1, \
		"CREATE TABLE demo (account TEXT, passwd TEXT)",\
		NULL, NULL, &errmsg)){
		printf("CREATE TABLE demo fail: %s\n", sqlite3_errmsg(db1));
	};

	sqlite3 *db2 = NULL;
	if(sqlite3_open("my.db", &db2)){
		printf("sqlite3_open fail: %s\n", sqlite3_errmsg(db2));
		return -1;
	}

	struct dbarg arg[] = {
		[0] = {
			.db = db1,
			.account = "jane",
			.passwd  = "123456",
		},
		[1] = {
			.db = db2,
			.account = "jack",
			.passwd  = "abcdef",
		},
	};

	pthread_t tid[2];
	pthread_create(&tid[0], NULL, do_datebase, (void *)&arg[0]);
	sleep(1);
	pthread_create(&tid[1], NULL, do_datebase, (void *)&arg[1]);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	sqlite3_close(db1);
	sqlite3_close(db2);
}

