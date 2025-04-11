/opt/gsoap-2.8/bin/soapcpp2 -S -L gSoapPrototipo.h;
/opt/gsoap-2.8/bin/wsdl2h -s -o wsModuloAfore.h wsModuloAfore.wsdl;
/opt/gsoap-2.8/bin/soapcpp2 -S -L wsModuloAfore.h;