#include<Stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
pthread_t thread1;
pthread_t thread2;
pthread_mutex_t mutex_lock;
void gotoxy(int x,int y);
void *elevator(void *);
void *operator(void *);
int running[6] = {0,0,0,0,0,0};
void select_elevator();
void option3();
void option4();
int cur_floor[6] = {1,1,11,11,1,1};
int floors2[6][20];
int updown[6] = {-1,-1,-1,-1,-1,-1};
int vip_mode[6] = {0,0,0,0,0,0};
int px,py; // 커서의 좌표
int simul_on = 1;
int main()
{
	system("clear");
	pthread_mutex_init(&mutex_lock,NULL);
	pthread_create(&thread1,NULL,&elevator,NULL);
	pthread_create(&thread2,NULL,&operator,NULL);
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		int option;
		printf("\033[0m");
		gotoxy(130,10);
		printf("┌───────── Options ─────────┐\n");
		gotoxy(130,11);
		printf("│ 1. 엘리베이터 이용        │\n");
		gotoxy(130,12);
		printf("│ 2. VIP 모드               │\n");
		gotoxy(130,13);
		printf("│ 3. 엘리베이터 점검        │\n");
		gotoxy(130,14);
		printf("│ 4. 시뮬레이션 정지/재시작 │\n");
		gotoxy(130,15);
		printf("│ 5. 시뮬레이션 종료        │\n");
		gotoxy(130,16);
		printf("└───────────────────────────┘\n");
		gotoxy(130,19);
		printf("select option    :   ");
		px = 148;
		py = 19;
		pthread_mutex_unlock(&mutex_lock);
		gotoxy(px,py);
		scanf("%d",&option);
		if(option == 1)
			select_elevator(option);
		if(option == 2)
			select_elevator(option);
		if(option == 3)
			option3();
		if(option == 4)
			option4();
		if(option == 5)
			break;
	}
	pthread_cancel(thread1);
	pthread_cancel(thread2);
	system("clear");
	return 0;
}
void *elevator(void *arg)
{
	while(1)
	{
		if(simul_on == 1)
		{
		int i,j;
		int y = 5;
		pthread_mutex_lock(&mutex_lock);
		for(i=20;i>0;i--)
		{
			for(j=0;j<6;j++)
			{	
				if(running[j] != -1 && cur_floor[j] == i)
					printf("\033[0;32m");
				else if(running[j] == -1 && cur_floor[j] == i)
					printf("\033[0;31m");
				else if(running[j] != -1 && floors2[j][i-1] == 1 && vip_mode[j] == 0)
					printf("\033[0;33m");
				else if(running[j] != -1 && floors2[j][i-1] == 2 && vip_mode[j] == 0)
					printf("\033[0;33m");
				else if(running[j] != -1 && floors2[j][i-1] == 3 && vip_mode[j] == 0)
					printf("\033[0;34m");
				else if(running[j] != -1 && floors2[j][i-1] == 4 && vip_mode[j] == 0)
					printf("\033[0;34m");
				else if(running[j] != -1 && floors2[j][i-1] == 5 && vip_mode[j] == 0)
					printf("\033[0;33m");
				else if(running[j] != -1 && floors2[j][i-1] == 6 && vip_mode[j] == 0)
					printf("\033[0;33m");
				else if(running[j] != -1 && vip_mode[j] == 1 && floors2[j][i-1]!=0)
					printf("\033[0;35m");
				else printf("\033[0m");
				gotoxy(10+j*20,y);
				if(i>=10) printf("┌─── %2dF ───┐\n",i);
				else printf("┌─── 0%dF ───┐\n",i);
				gotoxy(10+j*20,y+1);
				printf("└───────────┘\n");
			}
			y+=2;
		}
		printf("\033[0m");
		for(i=0;i<6;i++)
		{
			gotoxy(11+i*20,y);
			if(updown[i] == -1) printf("Elevator %d  ",i+1);
			if(updown[i] == 0) printf("Elevator %d ▲",i+1);
			if(updown[i] == 1) printf("Elevator %d ▼",i+1);
		}
		pthread_mutex_unlock(&mutex_lock);
		gotoxy(px,py);
		sleep(1);
		}
	}
}
void *operator(void *arg)
{
	int i,j,k;
	while(1)
	{
		if(simul_on == 1)
		{
		for(i=0;i<6;i++)
		{
			int flag1 = 0;
			int flag2 = 0;
			if(running[i] == 1 && updown[i] == 0)
			{
				if(floors2[i][cur_floor[i]-1] == 1)
					floors2[i][cur_floor[i]-1] = 0;
				for(j=cur_floor[i];j<20;j++)
				{
					if(floors2[i][j] == 1 || floors2[i][j] == 2 || floors2[i][j] == 5 || floors2[i][j] == 6)//
						flag1 = 1;
				}
				if(flag1 == 1) // 위로 올라가야 하는 경우 
					cur_floor[i] += 1;
				if(flag1 == 0) // 위로 올라가기를 멈춰야 하는 경우
				{
					for(j=0;j<20;j++)
					{
						if(floors2[i][j] != 0)
							flag2 = 1;
					}
					if(flag2 == 0)
					{
						running[i] = 0;
						updown[i] = -1;
					}
					if(flag2 == 1)
					{
						updown[i] = 1;
						for(j=0;j<20;j++)
						{
							if(floors2[i][j] == 3 || floors2[i][j] == 5)
								floors2[i][j] = 1;
						}
					}
				}
			}
			else if(running[i] == 1 && updown[i] == 1)
			{
				if(floors2[i][cur_floor[i]-1] == 2)
					floors2[i][cur_floor[i]-1] = 0;
				for(j=cur_floor[i]-2;j>=0;j--) //
				{
					if(floors2[i][j] == 1 || floors2[i][j] == 2 || floors2[i][j] == 5 || floors2[i][j] == 6)
						flag1 = 1;
				}
				if(flag1 == 1) // 아래로 내려가야 하는 경우
					cur_floor[i]-=1;
				if(flag1 == 0) // 아래로 내려가기를 멈춰야 하는 경우
				{
					for(j=0;j<20;j++)
					{
						if(floors2[i][j]!= 0)
							flag2 = 1;
					}
					if(flag2 == 0)
					{
						running[i] = 0;
						updown[i] = -1;
					}
					if(flag2 == 1)
					{
						updown[i] = 0;
						for(j=0;j<20;j++)
						{
							if(floors2[i][j] == 4 || floors2[i][j] == 6)
								floors2[i][j] = 2;
						}
					}
				}
			}
			int flag3 = 0;
			if(vip_mode[i] == 1)
			{
				for(j=0;j<20;j++)
					if(floors2[i][j]!=0)
						flag3 = 1;
				if(flag3 == 0)
					vip_mode[i] = 0;
			}
		}
		sleep(1);
		}
	}
}

void select_elevator(int option)
{
	int starting_point,destination,i,j,k;
	int available_elevator[6] = {0,0,0,0,0,0};
	int wt[6] = {-1,-1,-1,-1,-1,-1};
	int min_wt = 60;
	int ev_num;
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		gotoxy(130,19);
		printf("                                           \n");
		gotoxy(130,19);
		printf("Current Floor   :");
		gotoxy(148,19);
		pthread_mutex_unlock(&mutex_lock);
		scanf("%d",&starting_point);
		if(starting_point>20 || starting_point<1)
			continue;
		else break;
	}
	while(1)
	{
		pthread_mutex_lock(&mutex_lock);
		gotoxy(130,19);
		printf("                                           \n");
		gotoxy(130,19);
		printf("Destination Floor:");
		gotoxy(148,19);
		pthread_mutex_unlock(&mutex_lock);
		scanf("%d",&destination);
		if(destination>20 || destination<1 || destination == starting_point)
			continue;
		else break;
	}
	if(starting_point<=10 && destination<= 10)
	{
		available_elevator[0] = 1;
		available_elevator[1] = 1;
		available_elevator[4] = 1;
		available_elevator[5] = 1;
	}
	else if(starting_point > 10 && destination > 10)
	{
		
		available_elevator[2] = 1;
		available_elevator[3] = 1;
		available_elevator[4] = 1;
		available_elevator[5] = 1;
	}
	else
	{
		
		available_elevator[4] = 1;
		available_elevator[5] = 1;
	}
	for(i=0;i<6;i++)
	{
		if(available_elevator[i] == 1)
		{
			if(running[i] == 0)
				wt[i] = abs(starting_point - cur_floor[i]);
			if(running[i] == 1 && updown[i] == 0 && vip_mode[i] != 1)
			{
				if(starting_point>=cur_floor[i]) // 호출한 층수가 현재 위치보다 위에 위치
				{
					if((destination - starting_point) > 0) // 위로 올라가는 요청을 받은 경우
						wt[i] = starting_point - cur_floor[i];
					else // 아래로 내려가는 요청을 받은 경우
					{
						int top;
						for(j=19;j>=0;j--)
							if(floors2[i][j]== 1 ||floors2[i][j] == 2) top = j+1;
						wt[i] = (top - cur_floor[i]) + abs(top - starting_point);
					}
				}
				else // 호출한 층수가 현재 위치보다 아래에 위치
				{
					if((destination - starting_point)>0) // 위로 올라가는 요청을 받은 경우
					{
						int top,bottom;
						for(j=0;j<20;j++)
							if(floors2[i][j]==1 || floors2[i][j] == 2) bottom = j+1;
						for(j=19;j>=0;j--)
							if(floors2[i][j] == 1 || floors2[i][j] == 2) top = j+1;
						wt[i] = (top - cur_floor[i]) + (top - bottom) + abs(starting_point - bottom);
					}
					else // 아래로 내려가는 요청을 받은 경우
					{
						int top;
						for(j=19;j>=0;j--)
							if(floors2[i][j] == 1|| floors2[i][j] == 2) top = j+1;
						wt[i] = (top - cur_floor[i]) + abs(top - starting_point);
					}
				}
			}
			if(running[i] == 1 && updown[i] == 1 && vip_mode[i] != 1)
			{
				if(starting_point<=cur_floor[i]) // 호출한 층수가 현재 위치보다 아래에 위치
				{
					if((destination - starting_point)<0) // 아래로 내려가는 요청을 받은 경우
						wt[i] = abs(cur_floor[i] - starting_point);
					else // 위로 올라가는 요청을 받은 경우
					{
						int bottom;
						for(j=0;j<20;j++)
							if(floors2[i][j] == 1 || floors2[i][j] == 2) bottom = j+1;
						wt[i] = cur_floor[i] - bottom + abs(starting_point - bottom);
					}
				}
				else // 호출한 층수가 현재 위치보다 위에 위치
				{
					if((destination - starting_point)<0) // 아래로 내려가는 요청을 받은 경우
					{
						int top,bottom;
						for(j=0;j<20;j++)
							if(floors2[i][j] == 1 || floors2[i][j] == 2) bottom = j+1;
						for(j=19;j>=0;j--)
							if(floors2[i][j] == 1 || floors2[i][j] == 2) top = j+1;
						wt[i] = (cur_floor[i] - bottom) + (top - bottom) + abs(top - starting_point);
					}
					else // 위로 올라가는 요청을 받은 경우
					{
						int bottom;
						for(j=0;j<20;j++)
							if(floors2[i][j]==1 ||floors2[i][j] == 2) bottom = j+1;
						wt[i] = (cur_floor[i] - bottom) + abs(starting_point - bottom); //
					}
				}
			}
		}
	}
	int flag = 0;
	for(i=0;i<6;i++)
	{	
		if(wt[i]!=-1)
		{
			flag = 1;
			if(wt[i]<min_wt)
			{
				min_wt = wt[i];
				ev_num = i;
			}
		}
	}
	if(flag == 1)
	{
		if(running[ev_num] == 0)
		{		
			if(cur_floor[ev_num]<starting_point)
				updown[ev_num] = 0;
			if(cur_floor[ev_num]>starting_point)
				updown[ev_num] = 1;
			if(cur_floor[ev_num] == starting_point)
			{
				if(destination > cur_floor[ev_num])
					updown[ev_num] = 0;
				if(destination < cur_floor[ev_num])
					updown[ev_num] = 1;
			}
			if((destination - starting_point)>0)
			{
				floors2[ev_num][starting_point-1] = 1;
				floors2[ev_num][destination - 1] = 1;
			}
			if((destination - starting_point)<0)
			{
				
				floors2[ev_num][starting_point-1] = 2;
				floors2[ev_num][destination - 1] = 2;
			}
		}
		if(running[ev_num] == 1)
		{
			if(updown[ev_num] == 0) // 선택된 엘리베이터가 상승중인 경우
			{
				if((destination - starting_point)>0)
				{
					if(starting_point > cur_floor[ev_num])
					{
						floors2[ev_num][starting_point-1] = 1;
						floors2[ev_num][destination-1] = 1;
					}
					if(starting_point < cur_floor[ev_num])
					{
						if(floors2[ev_num][starting_point-1] == 1)
							floors2[ev_num][starting_point-1] = 5;
						else
							floors2[ev_num][starting_point-1] = 3;
						if(floors2[ev_num][destination-1] == 1)
							floors2[ev_num][destination-1] = 5;
						else
							floors2[ev_num][destination-1] = 3;
					}
					if(starting_point == cur_floor[ev_num])
						floors2[ev_num][destination-1] = 1;
				}
				if((destination - starting_point)<0)
				{
					floors2[ev_num][starting_point-1] = 2;
					floors2[ev_num][destination-1] = 2;
				}
			}
			if(updown[ev_num] == 1) // 선택된 엘리베이터가 하강중인 경우
			{
				if((destination - starting_point)<0) // 아래로 내려가는 요청을 받은 경우
				{
					if(starting_point < cur_floor[ev_num]) // 호출한 층수가 현재 위치보다 아래에 있는 경우
					{
						floors2[ev_num][starting_point-1] = 2;
						floors2[ev_num][destination-1] = 2;
					}
					if(starting_point > cur_floor[ev_num]) // 호출한 층수가 현재 위치보다 위에 있는 경우
					{
						if(floors2[ev_num][starting_point-1] == 2)
							floors2[ev_num][starting_point-1] = 6;
						else
							floors2[ev_num][starting_point-1] = 4;
						if(floors2[ev_num][destination-1] == 2)
							floors2[ev_num][destination-1] = 6;
						else
							floors2[ev_num][destination-1] = 4;
					}
					if(starting_point == cur_floor[ev_num])
						floors2[ev_num][destination-1] = 2;
				}
				if((destination - starting_point) >0) // 위로 올라가는 요청을 받은 경우
				{
					floors2[ev_num][starting_point-1] = 1;
					floors2[ev_num][destination-1] = 1;
				}
			}	
		}
		if(option == 2) vip_mode[ev_num] = 1;
		running[ev_num] = 1;
		pthread_mutex_lock(&mutex_lock);
		gotoxy(130,21);
		printf("                                              \n");
		gotoxy(130,21);
		printf("%d층에서 %d층으로 이동 요청\n",starting_point,destination);
		gotoxy(130,22);
		printf("                                              \n");
		gotoxy(130,22);
		printf("%d번 엘리베이터가 요청에 응답\n",ev_num+1);
		pthread_mutex_unlock(&mutex_lock);
	}
	if(flag == 0)
	{
		pthread_mutex_lock(&mutex_lock);
		gotoxy(130,21);
		printf("                                              \n");
		gotoxy(130,21);
		printf("현재 이용가능한 엘리베이터가 없습니다.\n");
		gotoxy(130,22); 
		printf("                                              \n");
		pthread_mutex_unlock(&mutex_lock);
	}
}
void option3()
{
	int ev,i;
	pthread_mutex_lock(&mutex_lock);
	gotoxy(130,19);
	printf("엘리베이터를 선택: ");
	px = 149;
	py = 19;
	gotoxy(px,py);
	pthread_mutex_unlock(&mutex_lock);
	scanf("%d",&ev);
	pthread_mutex_lock(&mutex_lock);
	gotoxy(130,21);
	printf("                                                   \n");
	gotoxy(130,22);
	printf("                                                   \n");
	gotoxy(130,21);
	if(running[ev-1] == 1)
		printf("선택한 엘리베이터는 현재 이용중 입니다.\n");
	else if(running[ev-1] == 0)
	{
		printf("%d 번 엘리베이터 점검을 시작합니다.\n",ev);
		running[ev-1] = -1;
	}
	else if(running[ev-1] == -1)
	{
		printf("%d 번 엘리베이터 점검을 종료합니다.\n",ev);
		running[ev-1] = 0;
	}
	pthread_mutex_unlock(&mutex_lock);
}
void option4()
{
	pthread_mutex_lock(&mutex_lock);
	if(simul_on == 1)
	{
		simul_on = 0;
		gotoxy(130,21);
		printf("                                            \n");
		gotoxy(130,22);
		printf("                                            \n");
		gotoxy(130,21);
		printf("시뮬레이션 일시정지\n");
	}
	else
	{
		simul_on = 1;
		gotoxy(130,21);
		printf("                                            \n");
		gotoxy(130,22);
		printf("                                            \n");
		gotoxy(130,21);
		printf("시뮬레이션 재시작\n");
	}
	pthread_mutex_unlock(&mutex_lock);
}
void gotoxy(int x,int y)
{
	printf("\033[%d;%df",y,x);
	fflush(stdout);
}
