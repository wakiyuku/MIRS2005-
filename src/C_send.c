#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
 
void C_send(int mode) {
    int sockfd;
    struct sockaddr_in addr;
 
    sleep(1);
    // ソケット生成
    if( (sockfd = socket( AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        perror( "socket" );
    }
 
    // 送信先アドレス・ポート番号設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 51000 );
    addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    //addr.sin_addr.s_addr = inet_addr( "172.21.2.167" );

    // サーバ接続
    connect( sockfd, (struct sockaddr *)&addr, sizeof( struct sockaddr_in ) );
 
    // データ送信
    char send_str[256];
    memset(send_str, '\0', sizeof(send_str));

    sprintf( send_str, "send  %d", mode);
    printf( "send:%d\n", mode );
    send( sockfd, send_str, 10, 0 ) ;
    sleep( 1 );
 
    // ソケットクローズ
    close( sockfd );
    return;
}
