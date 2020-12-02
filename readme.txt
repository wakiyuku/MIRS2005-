Ver3.0.1 README

------------------------------------------------------------
ver1.0との主な変更点
・シリアル通信の送信データをunsigned char×3からsigned short×3に変更
　これにより、負数や255超えの数も加工なしでやり取りできるようになった

var2.0との主な変更点
・Arduinoとのシリアル通信速度を 9600bps から 115200bps に変更した。
・超音波センサが１つでもテストを行えるようにした。

var3.0との変更点
・test_requet.c の次のように変更した
　- キーボード入力受付をscanf()を用いて簡略化した。
　- 直進要求、回転要求に対して、制御終了を待つようにした。
　- 5: quit の選択肢を入れた。


-------------------------------------------------------------------------

<io.c>
int io_open()
GPIOをオープン

void io_get_sw(int *sw_f, int *sw_l, int *sw_r)
バンパの測定値を取得

<uss.c>
int uss_open_l()
左の超音波センサをオープン

int uss_open_r()
右の超音波センサをオープン

long uss_get_l()
左の超音波センサの測定値を取得
long uss_get_r()

<arduino.c>
int  arduino_open()
Arduinoをオープン

void arduino_close()
Arduinoをクローズ

void arduino_clear()
Arduinoからの受信バッファをクリア

int  arduino_read(serial_data_t *serial_data)
serial_data_t型でデータを受信

void arduino_write(serial_data_t serial_data)
serial_data_t型でデータを送信

int  arduino_receive(command_data_t *command_data)
command_data_t型でデータを受信

void arduino_send(command_data_t command_data)
command_data_t型でデータを送信

middle_data_t  arduino_decode1(serial_data_t serial_data)
command_data_t arduino_decode2(middle_data_t middle_data)
serial_data_t  arduino_encode1(middle_data_t middle_data)
middle_data_t  arduino_encode2(command_data_t command_data)
各種データ型の変換

<request.c>
void request_set_runmode(run_state_t state, int speed, int dist)
走行モード、速度、距離を指定

int request_get_runmode(run_state_t *state, int *speed, int *dist)
走行モード、速度、距離を取得

int request_get_dist(int *dist_l, int *dist_r)
左右のタイヤの走行距離を取得

int request_get_batt(double *volt)
バッテリ電圧を取得

<direction.c>
int direction_correct(int dist)
距離distにて正対補正を実行

<get_img.c>
int get_img(const char *filename, int device, int width, int height)
uvccaptureにて画像を撮影

<number_search.c>
int  number_search(const IplImage *img_src)
入力画像に対しROI走査にて数字を認識

<number_detect.c>
void number_set_device(int device)
数字認識モジュールで使用するカメラの設定

void number_get(int *ans)
認識した数字を取得

int number_detect(int dist)
数字認識の実行

<color_binarize.c>
void color_binarize(const IplImage *img_src, IplImage *img_dst, int code, CvScalar lower, CvScalar upper)
色による画像の2値化

<circle_detect.c>
int circle_create()
風船認識スレッドを生成

void circle_cancel()
風船認識スレッドをキャンセル

void circle_set_device(int device)
風船認識モジュールで使用するカメラの設定

void circle_get_cg(int *x_g, int *y_g, int *ratio_g)
赤い部分の割合と重心座標を取得

void circle_get_cir(int *x_g, int *y_g, int *radius_g)
認識した円の半径と中心座標を取得

int circle_detect()
風船認識の実行

<server.c>
int server_info_number(int num)
数字データをサーバに送信

<position.c>
void position_set_coord(double x_s, double y_s, double dir_s)
座標データをセット

void position_straight(double dist)
直進距離を入力して座標データを更新

void position_rotate(double angle)
回転角度を入力して座標データを更新

void position_correct(double dist)
正対補正距離を入力して座標データを更新

void position_get_coord(double *x_g, double *y_g, double *dir_g)
座標データを取得

char position_get_area()
現在エリアを取得

