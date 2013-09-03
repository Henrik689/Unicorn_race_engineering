include $(TOPDIR)/rules.mk
PKG_NAME:=datalog
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install
include $(TOPDIR)/package/rules.mk
$(eval $(call PKG_template,HELLO,$(PKG_NAME),$(PKG_VERSION)-$(PKG_RELEASE),$(ARCH)))
$(PKG_BUILD_DIR)/.configured: $(PKG_BUILD_DIR)/.prepared
        #Since there is no configure script, we can directly go to the building step
	touch $@
$(PKG_BUILD_DIR)/.built:
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf build
	mkdir -p $(PKG_INSTALL_DIR)/usr/bin
	mkdir build
	$(TARGET_CC) src/$(PKG_NAME).c -o $(PKG_BUILD_DIR)/$(PKG_NAME) ## -lyourlib #Note we directly call the cross-compiler and define its output
	echo hest
	$(CP) $(PKG_BUILD_DIR)/$(PKG_NAME) $(PKG_INSTALL_DIR)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/$(PGK_NAME) build/
	touch $@
$(IPKG_HELLO):
	install -d -m0755 $(IDIR_HELLO)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/$(PKG_NAME) $(IDIR_HELLO)/usr/bin
	$(RSTRIP) $(IDIR_HELLO)
	$(IPKG_BUILD) $(IDIR_HELLO) $(PACKAGE_DIR)
mostlyclean:
	make -C $(PKG_BUILD_DIR) clean
	rm $(PKG_BUILD_DIR)/.built

