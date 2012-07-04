#include <stdio.h>
#include "mongo.h"

int main() {
mongo conn;
int n;


bson b[1];
bson c;

 bson_init( b );
 bson_append_new_oid( b, "_id" );
bson_append_string( b, "name", "Joe" );
bson_append_int( b, "age", 33 );
 bson_finish( b );

 
    /* Now make a connection to MongoDB. */
    if( mongo_connect( &conn, "127.0.0.1", 27017 ) != MONGO_OK ) {
      switch( conn.err ) {
        case MONGO_CONN_NO_SOCKET:
          printf( "FAIL: Could not create a socket!\n" );
          break;
        case MONGO_CONN_FAIL:
          printf( "FAIL: Could not connect to mongod. Make sure it's listening at 127.0.0.1:27017.\n" );
          break;
      }

      exit( 1 );
    }


 n=mongo_insert( &conn, "test.foo", b );

printf("n=%i, err=%i\n", n, conn.err);

 n=mongo_insert( &conn, "test.foo", b );

printf("n=%i, err=%i\n", n, conn.err);


 n=mongo_insert( &conn, "test.hey", b );

printf("n=%i, err=%i\n", n, conn.err);


return 0;
}

