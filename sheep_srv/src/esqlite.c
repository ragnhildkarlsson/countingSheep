#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sqlite3.h>

#define BUF_SIZE 1000
#define SQL_SIZE 5000
#define TMPSQL_SIZE 400
#define MAX_FARMDATA 500
#define MAX_USERDATA 20
#define FARM_NAME 25
#define FARM_DESC 300
#define ANTY_TYPE 25
#define USER_NAME 25

struct farmdata{
	int		counter;
	int		farm_fid;
	char	farm_name[FARM_NAME+1];
	char	farm_description[FARM_DESC+1];
	int		anty_tid;
	char	anty_type[ANTY_TYPE+1];
	int		anim_aid;
	int		fami_parent;
};

struct userdata{
	int		counter;
	int		uid;
	char	name[USER_NAME+1];
};

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char **argv){
	sqlite3 *db;
	int rc, i, nof_record=0, j, infinit_loop=1000;
	char buf[BUF_SIZE + 1], *zErrMsg = 0, *token, *string, *tofree, **ptr, host[16], delim[2]=",";
	FILE * inp;


	if(argc == 2){ 
		inp = fopen(argv[1], "r");
		if(!inp){
			perror(argv[1]);
			return EXIT_FAILURE;
		}
	}else{
		inp = stdin;
	}
	/* read lines from stream */
	while(fgets(buf, BUF_SIZE, inp)){
		/* do we have a macth? */
		if(!strncmp(buf,"HOST",4)){
			/* chop up the line in tokens and extract data*/
			tofree = string = strdup(buf);
			assert(string != NULL);
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error HOST: %s\n", buf);
				return EXIT_FAILURE;
			}
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error HOST: %s\n", buf);
				return EXIT_FAILURE;
			}
			strcpy(host, token);
			fprintf(stdout, "HOST:\n\t%s\n", host);
			free(tofree);
		} else if(!strncmp(buf,"FARMDATA",8)){
			struct farmdata fdata[MAX_FARMDATA];
			/* chop up the line in tokens and extract data*/
			tofree = string = strdup(buf);
			assert(string != NULL);
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error FARMDATA: %s\n", buf);
				return EXIT_FAILURE;
			}
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error FARMDATA: %s\n", buf);
				return EXIT_FAILURE;
			}
			nof_record = atoi(token);
			free(tofree);
			/* walk through rest of stream line by line */
			fprintf(stdout, "FARMDATA:\n");
			i=j=0;
			while(i<nof_record && j<infinit_loop){
				j++;
				if(fgets(buf, BUF_SIZE, inp)!=NULL){
					/* ignore comments */
					if(buf[0]!='#'){
						/* remove newline */
						buf[strcspn(buf, "\r\n")] = '\0';
						/* chop up the lines in tokens and extract data*/
						tofree = string = strdup(buf);
						assert(string != NULL);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						fdata[i].counter = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						fdata[i].farm_fid = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						strcpy(fdata[i].farm_name, token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						strcpy(fdata[i].farm_description, token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						fdata[i].anty_tid = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						strcpy(fdata[i].anty_type, token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						fdata[i].anim_aid = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						fdata[i].fami_parent = atoi(token);
						fprintf(stdout, "\t%d\t%d\t%s\t%s\t%d\t%s\t%d\t%d\n", fdata[i].counter, fdata[i].farm_fid, fdata[i].farm_name, fdata[i].farm_description, fdata[i].anty_tid, fdata[i].anty_type, fdata[i].anim_aid, fdata[i].fami_parent);
						free(tofree);
						i++;
					}
				}
			}
			char tmpsql[TMPSQL_SIZE+1], sql[SQL_SIZE+1];
			strcpy(sql, "INSERT INTO transferdata(counter, farm_fid, farm_name, farm_description, anty_tid, anty_type, anim_aid, fami_parent) VALUES ");
			i=0;
			while(i<nof_record){
				if(i==nof_record-1)
					sprintf(tmpsql, "(%d,%d,'%s','%s',%d,'%s',%d,%d);", fdata[i].counter, fdata[i].farm_fid, fdata[i].farm_name, fdata[i].farm_description, fdata[i].anty_tid, fdata[i].anty_type, fdata[i].anim_aid, fdata[i].fami_parent);
				else
					sprintf(tmpsql, "(%d,%d,'%s','%s',%d,'%s',%d,%d),", fdata[i].counter, fdata[i].farm_fid, fdata[i].farm_name, fdata[i].farm_description, fdata[i].anty_tid, fdata[i].anty_type, fdata[i].anim_aid, fdata[i].fami_parent);
				strcat(sql,tmpsql);
				i++;
			}
			printf("%s\n", sql);
			rc = sqlite3_open("sheep.db", &db);
			if( rc ){
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(sql, "INSERT INTO farmlist(fid,name,description) SELECT DISTINCT farm_fid, farm_name, farm_description FROM transferdata;");
			printf("%s\n", sql);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(sql, "INSERT INTO animaltype(tid, type) SELECT DISTINCT anty_tid, anty_type FROM transferdata;");
			printf("%s\n", sql);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(sql, "INSERT INTO animal(aid,tid) SELECT DISTINCT anim_aid, anty_tid FROM transferdata;");
			printf("%s\n", sql);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(sql, "INSERT INTO familylist(fid,aid,parent) SELECT DISTINCT farm_fid, anim_aid, fami_parent FROM transferdata;");
			printf("%s\n", sql);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			sqlite3_close(db);
		} else if(!strncmp(buf,"USERDATA",8)){
			struct userdata udata[MAX_USERDATA];
			/* chop up the line in tokens and extract data*/
			tofree = string = strdup(buf);
			assert(string != NULL);
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error USERDATA: %s\n", buf);
				return EXIT_FAILURE;
			}
			if((token = strsep(&string, delim)) == NULL){
				fprintf(stderr, "Read error USERDATA: %s\n", buf);
				return EXIT_FAILURE;
			}
			nof_record = atoi(token);
			free(tofree);
			/* walk through rest of stream line by line */
			fprintf(stdout, "USERDATA:\n");
			i=j=0;
			while(i<nof_record && j<infinit_loop){
				j++;
				if(fgets(buf, BUF_SIZE, inp)!=NULL){
					/* ignore comments */
					if(buf[0]!='#'){
						/* remove newline */
						buf[strcspn(buf, "\r\n")] = '\0';
						/* chop up the lines in tokens and extract data*/
						tofree = string = strdup(buf);
						assert(string != NULL);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						udata[i].counter = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						udata[i].uid = atoi(token);
						if((token = strsep(&string, delim)) == NULL){
							fprintf(stderr, "Read error FARMDATA: %s\n", buf);
							return EXIT_FAILURE;
						}
						strcpy(udata[i].name, token);
						fprintf(stdout, "\t%d\t%d\t%s\n", udata[i].counter, udata[i].uid, udata[i].name);
						free(tofree);
						i++;
					}
				}
			}
			char tmpsql[TMPSQL_SIZE+1], sql[SQL_SIZE+1];
			strcpy(sql, "INSERT INTO user(uid,name) VALUES ");
			i=0;
			while(i<nof_record){
				if(i==nof_record-1)
					sprintf(tmpsql, "(%d,'%s')", udata[i].uid, udata[i].name);
				else
					sprintf(tmpsql, "(%d,'%s'),", udata[i].uid, udata[i].name);
				strcat(sql,tmpsql);
				i++;
			}
			printf("%s\n", sql);
			rc = sqlite3_open("sheep.db", &db);
			if( rc ){
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			sqlite3_close(db);
		} else if(!strncmp(buf,"DELETEALL",9)){
			char tmpsql[TMPSQL_SIZE+1];
			rc = sqlite3_open("sheep.db", &db);
			if( rc ){
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
				sqlite3_close(db);
				return(1);
			}
			strcpy(tmpsql, "DELETE FROM transferdata");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(tmpsql, "DELETE FROM user");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(tmpsql, "DELETE FROM familylist");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(tmpsql, "DELETE FROM animal");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(tmpsql, "DELETE FROM animaltype");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			strcpy(tmpsql, "DELETE FROM farmlist");
			printf("%s\n", tmpsql);
			rc = sqlite3_exec(db, tmpsql, callback, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			sqlite3_close(db);
		}
	}
	fclose(inp);
	return EXIT_SUCCESS;
}
