CFLAGS= -c -g
LDFLAGS= -lpthread -levent
# LDFLAGS= -lpthread 

OBJS += yuyv2yyuv			
OBJS += yuvuv2yuuvv
OBJS += yuyv2uyvy

all:$(OBJS)
.PHONY: all

%.o:%.c
	cc $(CFLAGS) $< -o $@

%.o:%.cc
	g++ $(CFLAGS) $< -o $@

yuyv2yyuv: yuyv2yyuv.cc
	g++ $^ -o $@ $(LDFLAGS) -g

yuyv2uyvy: yuyv2uyvy.cc
	g++ $^ -o $@ $(LDFLAGS) -g

yuvuv2yuuvv: yuvuv2yuuvv.cc
	g++ $^ -o $@ $(LDFLAGS) -g


clean:
	rm *.o 
	rm $(OBJS)


