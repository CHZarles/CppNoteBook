#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void timeout(int sig) // 信号处理器
{
  if (sig == SIGALRM)
    puts("Time out!");

  alarm(2); // 为了每隔 2 秒重复产生 SIGALRM 信号，在信号处理器中调用 alarm
  // 函数
}
void keycontrol(int sig) // 信号处理器

{
  if (sig == SIGINT)
    puts("CTRL+C pressed");
}
int main(int argc, char *argv[]) {
  int i;
  signal(SIGALRM, timeout); // 注册信号及相应处理器
  signal(SIGINT, keycontrol);
  alarm(2); // 预约 2 秒候发生 SIGALRM 信号

  for (i = 0; i < 3; i++) {
    puts("wait...");
    sleep(100);
  }
  return 0;
  /*
  调用函数的主体的确是操作系统，但是进程处于睡眠状态时无法调用函数，因此，产生信号时，
  为了调用信号处理器，将唤醒由于调用 sleep 函数而进入阻塞状态的进程。
  而且，进程一旦被唤醒，就不会再进入睡眠状态。即使还未到 sleep
  中规定的时间也是如此。 所以上述示例运行不到 10 秒后就会结束，连续输入 CTRL+C
  可能连一秒都不到。 简言之，就是本来系统要睡眠100秒，但是到了 alarm(2)
  规定的两秒之后，就会唤醒睡眠的进程，进程被唤醒了就不会再进入睡眠状态了，所以就不用等待100秒。
  如果把 timeout() 函数中的 alarm(2) 注释掉，就会先输出wait...，然后再输出Time
  out! (这时已经跳过了第一次的 sleep(100)
  秒),然后就真的会睡眠100秒，因为没有再发出 alarm(2) 的信号。
  */
}
