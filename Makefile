SUBDIRS = camera server
TARGETDIR = obj bin
PREFIX = /usr/local

.PHONY: subdirs $(SUBDIRS) targerdir $(TARGETDIR)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

install:
	cp bin/server $(PREFIX)/bin/
	cp bin/camera $(PREFIX)/bin/
	cp share/trained_cascade.xml $(PREFIX)/share/

uninstall:
	rm $(PREFIX)/bin/server
	rm $(PREFIX)/bin/camera
	rm $(PREFIX)/share/trained_cascade.xml

clean:
	rm obj/camera.o obj/mutex.o obj/open_kinect.o obj/server.o 
	rm bin/server bin/camera
