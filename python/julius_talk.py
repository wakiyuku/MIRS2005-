# coding: UTF-8

import socket
import time
import subprocess
import voice
#モジュールモード起動関数
def julius_on():
    julius = ['julius']
    jconf1 = ['-C','./../../Julius/dictation-kit-4.5/main.jconf']#jconf設定ファイルを読み込む．ファイルの内容がこの場所に展開される．
    jconf2 = ['-C','./../../Julius/dictation-kit-4.5/am-gmm.jconf']
    option = ['-module','-demo']
    cmd = julius + jconf1 + jconf2 + option
    subprocess.Popen(cmd,stdin=subprocess.PIPE)
#音声認識関数
def julius():
    time.sleep(3)
    host = '127.0.0.1'   # juliusサーバーのIPアドレス
    port = 10500         # juliusサーバーの待ち受けポート
    data_size = 1024     # 受信データバイト数
    # socket通信でjuliusサーバーに接続（接続要求）
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
 
        strtemp = ""#話した言葉を格納する変数
        fin_flag = False #話終わりのフラグ
 
        
        while True:
            # juliusサーバからデータ受信
            data = s.recv(data_size).decode('utf-8')
 
            for line in data.split('\n'):
                # 受信データから、<WORD>の後に書かれている言葉を抽出して変数に格納する。
                # <WORD>の後に、話した言葉が記載されている。
                index = line.find('WORD="')
                if index != -1:#認識した文字列があれば
                    #strtempに話した言葉を格納
                    strtemp = strtemp + line[index+6:line.find('"',index+6)]
                # 受信データに</RECOGOUT>'があれば、話終わり ⇒ フラグをTrue    
                if '</RECOGOUT>' in line:
                    fin_flag = True
                if '<RECOGFAIL/> ' in line:
                    print("聞き取れませんでした")
                    fin_flag = False
 
            if fin_flag == True:
                if "いい天気。" in strtemp:
                    voice.jtalk("そうですね")
                    print(strtemp)
                elif "調子は。" in strtemp:
                    voice.jtalk("元気ですよ")
                    print(strtemp)
                elif "写真。" in strtemp:
                    voice.jtalk("写真をとります,３，２，１")
                    print(strtemp)
                elif "名前は。" in strtemp:
                    voice.jtalk("そらしといいます")
                    print(strtemp)
                else:
                    print("もう一度言ってください")
                    #voice.jtalk("もう一度言ってください")
                    
            
                fin_flag = False
                strtemp = ""
                
if __name__ == "__main__":
    julius_on()
    julius()
