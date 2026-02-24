#include "header_file\\MidiFun.h"
#include "header_file\\particle.h"
#include <algorithm>
#include <math.h>
#include <iostream>
using namespace std;
using namespace particle;
#define PI 3.1415926535897931
MidiFun midi;
NOTE note;
NOTEev pinit(-10, -1, -1, 64, -1, -1);
FILE *play, *reset;
long long tickMax, tickMin;
int pitchMax, pitchMin;
int num;
template <typename __FUNCTION>
void drawline(NOTE &lp, NOTE &np, __FUNCTION writecmd)
{
	int lpn = lp.size(), npn = np.size();
	if (lpn * 1.0 / npn > 1)
	{
		int k = lpn / npn;
		if (k * npn == lpn)
			for (int i = 1; i <= npn; i++)
				for (int j = k * (i - 1) + 1; j <= k * i; j++)
					writecmd(lp[j - 1], np[i - 1]);
		else
		{
			for (int i = 1; i <= lpn - k * npn; i++)
				for (int j = (k + 1) * (i - 1) + 1; j <= (k + 1) * i; j++)
					writecmd(lp[j - 1], np[i - 1]);
			for (int i = lpn - k * npn + 1; i <= npn; i++)
				for (int j = k * (i - 1 - npn) + lpn + 1; j <= k * (i - npn) + lpn; j++)
					writecmd(lp[j - 1], np[i - 1]);
		}
	}
	else if (npn * 1.0 / lpn > 1)
	{ //npn?lpn??k??(k>1)
		int k = npn / lpn;
		if (k * lpn == npn)
			for (int i = 1; i <= lpn; i++)
				for (int j = k * i - k + 1; j <= k * i; j++)
					writecmd(lp[i - 1], np[j - 1]);
		else
		{
			for (int i = 1; i <= npn - k * lpn; i++)
				for (int j = (k + 1) * (i - 1) + 1; j <= (k + 1) * i; j++)
					writecmd(lp[i - 1], np[j - 1]);
			for (int i = npn - k * lpn + 1; i <= lpn; i++)
				for (int j = k * (i - 1 - lpn) + npn + 1; j <= k * (i - lpn) + npn; j++)
					writecmd(lp[i - 1], np[j - 1]);
		}
	}
	else //lpn????npn
		for (int i = 1; i <= npn; i++)
			writecmd(lp[i - 1], np[i - 1]);
}
template <typename __FUNCTION>
void makeline(NOTEev &pinit, __FUNCTION writecmd)
{
	NOTE lp, np;
	lp.push(pinit);
	np.push(note[0]);
	int size = note.size();
	for (int i = 1; i < size; i++)
	{
		if(note[i].tick==-100||note[i].volume==0)
			continue;
		if (
			np[0].track != note[i].track)
		{
			drawline(lp, np, writecmd);
			lp.clear();
			np.clear();
			lp.push(pinit);
			np.push(note[i]);
		}
		else if (
			np[0].track == note[i].track &&
			np[0].tick != note[i].tick)
		{
			drawline(lp, np, writecmd);
			lp.clear();
			lp.move(np);
			np.push(note[i]);
		}
		else
			np.push(note[i]);
	}
	drawline(lp, np, writecmd);
}
void init(void)
{
	reset = fopen("functions\\reset.mcfunction", "w");
	play = fopen("functions\\play.mcfunction", "w");
	midi.init("midi\\告白气球.mid");
	midi.get_note(note);
	note.sort();
	for (int i = 0; i < note.size(); i++)
	{
		if (note[i].channel == 8 || note[i].channel == 6)
		{
			if(note[i].channel==6){
				note[i].pitch-=12;
			}
			tickMax = max(tickMax, note[i].tick);
			tickMin = min(tickMin, note[i].tick);
			pitchMax = max(pitchMax, note[i].pitch);
			pitchMin = min(pitchMin, note[i].pitch);
		}
		else if(note[i].channel==0){
		}
		else if(note[i].channel!=9||(note[i].pitch!=36&&note[i].pitch!=38)){
			note[i].tick=-100;
		}
	}
}
void writecmd(NOTEev &a, NOTEev &b)
{
	if (b.volume == 0)
		return;
	if (b.channel != 0 && b.channel != 8 && b.channel != 6&& b.channel != 9)
		return;
	num++;
	if (b.channel == 8)
	{
		if (a.tick < 1307 || (a.tick >= 1759 && a.tick < 3033) || (a.tick >= 3465))
		{
			double len = sqrt(pow(b.tick - a.tick, 2) + pow(b.pitch - a.pitch, 2));
			useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
						String<const char *, int, const char *, int, const char *>("particles\\particles\\channel8TriFun", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
						String<const char *, int, const char *, int>("channel8TriFun", b.tick - a.tick, "_", b.pitch - a.pitch),
						(b.tick - a.tick) / 4.0,
						Offset(String<int, const char *>(b.tick - a.tick, "*T"),
							   String<const char *, double, const char *, double>("Math.cos(360*T+180)*", len / 6, "+", len / 6),
							   String<int, const char *>(b.pitch - a.pitch, "*T")),
						15,
						Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
							 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
						VariousColor({{0, "#9CECFB"},
									  {0.5, "#EC6EAD"},
									  {1, "#0052d4"}}));
			fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel8TriFun%d_%d %d 200 %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch);
		}
		else if ((a.tick >= 1307 && a.tick < 1759 ) || (a.tick >= 3033  && a.tick < 3465 ))
		{
			double len = sqrt(pow(b.tick - a.tick, 2) + pow(b.pitch - a.pitch, 2));
			useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
						String<const char *, int, const char *, int, const char *>("particles\\particles\\channel8Parabola", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
						String<const char *, int, const char *, int>("channel8Parabola", b.tick - a.tick, "_", b.pitch - a.pitch),
						(b.tick - a.tick) / 4.0,
						Offset(String<int, const char *>(b.tick - a.tick, "*T"),
							   String<double, const char *>(-len/6, "*T*(T-1)"),
							   String<int, const char *>(b.pitch - a.pitch, "*T")),
						15,
						Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
							 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
						VariousColor({{0, "#9CECFB"},
									  {0.5, "#EC6EAD"},
									  {1, "#0052D4"}}));
			fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel8Parabola%d_%d %d 200 %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch);
		}
	}
	else if (b.channel == 6)
	{
		if (a.tick < 1307 || (a.tick >= 1759 && a.tick < 3033) || (a.tick >= 3465 ))
		{
			double len = sqrt(pow(b.tick - a.tick, 2) + pow(b.pitch - a.pitch, 2));
			useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
						String<const char *, int, const char *, int, const char *>("particles\\particles\\channel6TriFun", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
						String<const char *, int, const char *, int>("channel6TriFun", b.tick - a.tick, "_", b.pitch - a.pitch),
						(b.tick - a.tick) / 4.0,
						Offset(String<int, const char *>(b.tick - a.tick, "*T"),
							   String<const char *, double, const char *, double>("Math.cos(360*T+180)*", len / 6, "+", len / 6),
							   String<int, const char *>(b.pitch - a.pitch, "*T")),
						15,
						Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
							 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
						VariousColor({{0, "#FDB99B"},
									  {0.5, "#CF8BF3"},
									  {1, "#A770EF"}}));
			fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel6TriFun%d_%d %d 200 %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch);
		}
		else if ((a.tick >= 1307  && a.tick < 1759  ) || (a.tick >= 3033   && a.tick < 3465  ))
		{
			double len = sqrt(pow(b.tick - a.tick, 2) + pow(b.pitch - a.pitch, 2));
			useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
						String<const char *, int, const char *, int, const char *>("particles\\particles\\channel6Parabola", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
						String<const char *, int, const char *, int>("channel6Parabola", b.tick - a.tick, "_", b.pitch - a.pitch),
						(b.tick - a.tick) / 4.0,
						Offset(String<int, const char *>(b.tick - a.tick, "*T"),
							   String<double, const char *>(-len/6, "*T*(T-1)"),
							   String<int, const char *>(b.pitch - a.pitch, "*T")),
						15,
						Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
							 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
						VariousColor({{0, "#FDB99B"},
									  {0.5, "#CF8BF3"},
									  {1, "#A770EF"}}));
			fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel6Parabola%d_%d %d 200 %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch);
		}
	}
	else if (b.channel == 0)
	{
		double len = sqrt(pow(b.tick - a.tick, 2) + pow(b.pitch - a.pitch, 2));
		useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
					String<const char *, int, const char *, int, const char *>("particles\\particles\\channel0Line", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
					String<const char *, int, const char *, int>("channel0Line", b.tick - a.tick, "_", b.pitch - a.pitch),
					(b.tick - a.tick) / 4.0,
					Offset(String<int, const char *>(b.tick - a.tick, "*T"),
						   String<int>(0),
						   String<int, const char *>(b.pitch - a.pitch, "*T")),
					15,
					Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
						 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
					VariousColor({{0, "#F4C4F3"}, {1, "#FC67FA"}}));
		fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel0Line%d_%d %d 230 %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch);
	}
	else if(b.channel==9){
		useTemplate(String<const char *>("particles\\template\\[线]通用.template"),
					String<const char *, int, const char *, int, const char *>("particles\\particles\\channel9Line", b.tick - a.tick, "_", b.pitch - a.pitch, ".json"),
					String<const char *, int, const char *, int>("channel9Line", b.tick - a.tick, "_", b.pitch - a.pitch),
					(b.tick - a.tick) / 4.0,
					Offset(String<int, const char *>(b.tick - a.tick, "*T"),
						   String<int, const char *>(b.pitch - a.pitch, "*T"),
						   String<int>(0)),
					5,
					Size(String<const char *>("(T>0.75?0.8*(1.25-T):0.4)"),
						 String<const char *>("(T>0.75?0.8*(1.25-T):0.4)")),
					Color(255,215,0,1));
		fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel9Line%d_%d %d %d %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch-36+214,pitchMax+5);
		fprintf(play, "execute @a[scores={tick=%d}] ~~~ particle channel9Line%d_%d %d %d %d\n", a.tick, b.tick - a.tick, b.pitch - a.pitch, a.tick, a.pitch-36+214,pitchMin-5);
	}
}
void makepoint(void)
{
	//
	useTemplate(String<const char *>("particles\\template\\[??]???????instant.template"),
				String<const char *>("particles\\particles\\coneInstant.json"),
				String<const char *>("coneInstant"),
				25,
				Color(255, 215, 0, 1));
	//?2????
	useTemplate(String<const char *>("particles\\template\\[??]?????.template"),
				String<const char *>("particles\\particles\\sphere.json"),
				String<const char *>("sphere"),
				25,
				Color(255, 215, 0, 1));
	//??1????
	for (int angle = 0; angle < 360; angle += 5)
	{
		useTemplate(String<const char *>("particles\\template\\[??]???????.template"),
					String<const char *, int, const char *>("particles\\particles\\circle", angle, ".json"),
					String<const char *, int>("circle", angle),
					25,
					Motion(String<double>(sin(PI * angle / 180)),
						   String<int>(0),
						   String<double>(cos(PI * angle / 180)),
						   String<int>(4)),
					Color(255, 215, 0, 1));
	}
	auto circle = [&](NOTEev a) {
		for (int i = 0; i < 360; i += 5)
		{
			fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle circle%d %d %d %d\n", a.tick, a.tick, i, a.tick, 200, a.pitch);
		}
	};
	//??2????
	useTemplate(String<const char *>("particles\\template\\[??]????????.template"),
				String<const char *>("particles\\particles\\slowlyRising.json"),
				String<const char *>("slowlyRising"),
				25,
				Color(255, 215, 0, 1));
	auto risingBlock = [&](NOTEev n) {
		struct A
		{
			int x, y, z;
		} a[] = {{0, 0, 0}, {0, 0, 1}, {0, 0, 2}, {0, 0, 3}, {0, 0, 4}, {0, 0, 5}, {0, 0, 6}, {0, 0, 7}, {0, 0, 8}, {0, 0, 9}, {0, 0, 10}, {0, 1, 0}, {0, 1, 10}, {0, 2, 0}, {0, 2, 10}, {0, 3, 0}, {0, 3, 10}, {0, 4, 0}, {0, 4, 10}, {0, 5, 0}, {0, 5, 10}, {0, 6, 0}, {0, 6, 10}, {0, 7, 0}, {0, 7, 10}, {0, 8, 0}, {0, 8, 10}, {0, 9, 0}, {0, 9, 10}, {0, 10, 0}, {0, 10, 1}, {0, 10, 2}, {0, 10, 3}, {0, 10, 4}, {0, 10, 5}, {0, 10, 6}, {0, 10, 7}, {0, 10, 8}, {0, 10, 9}, {0, 10, 10}, {1, 0, 0}, {1, 0, 10}, {1, 10, 0}, {1, 10, 10}, {2, 0, 0}, {2, 0, 10}, {2, 10, 0}, {2, 10, 10}, {3, 0, 0}, {3, 0, 10}, {3, 10, 0}, {3, 10, 10}, {4, 0, 0}, {4, 0, 10}, {4, 10, 0}, {4, 10, 10}, {5, 0, 0}, {5, 0, 10}, {5, 10, 0}, {5, 10, 10}, {6, 0, 0}, {6, 0, 10}, {6, 10, 0}, {6, 10, 10}, {7, 0, 0}, {7, 0, 10}, {7, 10, 0}, {7, 10, 10}, {8, 0, 0}, {8, 0, 10}, {8, 10, 0}, {8, 10, 10}, {9, 0, 0}, {9, 0, 10}, {9, 10, 0}, {9, 10, 10}, {10, 0, 0}, {10, 0, 1}, {10, 0, 2}, {10, 0, 3}, {10, 0, 4}, {10, 0, 5}, {10, 0, 6}, {10, 0, 7}, {10, 0, 8}, {10, 0, 9}, {10, 0, 10}, {10, 1, 0}, {10, 1, 10}, {10, 2, 0}, {10, 2, 10}, {10, 3, 0}, {10, 3, 10}, {10, 4, 0}, {10, 4, 10}, {10, 5, 0}, {10, 5, 10}, {10, 6, 0}, {10, 6, 10}, {10, 7, 0}, {10, 7, 10}, {10, 8, 0}, {10, 8, 10}, {10, 9, 0}, {10, 9, 10}, {10, 10, 0}, {10, 10, 1}, {10, 10, 2}, {10, 10, 3}, {10, 10, 4}, {10, 10, 5}, {10, 10, 6}, {10, 10, 7}, {10, 10, 8}, {10, 10, 9}, {10, 10, 10}};
		for (auto i : a)
		{
			fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle slowlyRising %lf %lf %lf\n", n.tick, n.tick, n.tick + i.x / 10.0, 200 + i.y / 10.0, n.pitch + i.z / 10.0);
		}
	};
	for (int i = 0; i < note.size(); i++)
	{
		if (note[i].volume == 0 || note[i].track == 1 || note[i].channel == 3)
		{
			continue;
		}
		if (note[i].channel == 9)
		{
			//cang~
			if (note[i].pitch == 49)
			{
				for (int angle = 0; angle < 360; angle += 5)
					fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle goldenPoint %d %lf %lf\n", note[i].tick, note[i].tick, note[i].tick + 5, 220.5 + sin(PI * angle / 180) * 2, 64.5 + cos(PI * angle / 180) * 2);
			}
			//pa~
			else if (note[i].pitch == 39)
			{
				for (int angle = 0; angle < 360; angle += 5)
					fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle redPoint %d %lf %lf\n", note[i].tick, note[i].tick, note[i].tick + 5, 220.5 + sin(PI * angle / 180) * 2, 64.5 + cos(PI * angle / 180) * 2);
			}
			continue;
		}
		fprintf(play, "%d\t execute @a[scores={tick=%d}] ~~~ setblock %d 200 %d air\n", note[i].tick, note[i].tick, note[i].tick, note[i].pitch);
		fprintf(reset, "setblock %d 200 %d redstone_block\n", note[i].tick, note[i].pitch);
		if (note[i].tick < 640 || note[i].tick >= 4160)
		{
			fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle coneSteady %d %d %d\n", note[i].tick, note[i].tick, note[i].tick, 200, note[i].pitch);
		}
		else if ((note[i].tick >= 640 && note[i].tick < 1280) || (note[i].tick >= 2560 && note[i].tick < 3200))
		{
			circle(note[i]);
		}
		else if ((note[i].tick >= 1280 && note[i].tick < 1935) || (note[i].tick >= 3200 && note[i].tick < 4160))
		{
			risingBlock(note[i]);
		}
		else if (note[i].tick >= 1935 && note[i].tick < 2560)
		{
			fprintf(play, "%d\texecute @a[scores={tick=%d}] ~~~ particle sphere %d %d %d\n", note[i].tick, note[i].tick, note[i].tick, 200, note[i].pitch);
		}
	}
}
/**********
track 2:??
track 3:??
**********/
void makeParticle(void)
{
	makeline(pinit, writecmd);
	//makepoint();
}
void end(void)
{
	fclose(play);
	fclose(reset);
}
int main(void)
{
	init();
	makeParticle();
	end();
	return 0;
}

