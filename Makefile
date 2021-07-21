all: test_timestamp

test_timestamp: test_timestamp.P timestamp.xwam
	xsb --nobanner --quietload test_timestamp

timestamp.xwam: timestamp.c timestamp.H
	xsb --nobanner --quietload timestamp

clean:
	rm -f timestamp.so timestamp.dll timestamp.dynlib timestamp.xwam test_timestamp.xwam

.PHONY: test_timestamp
