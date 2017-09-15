/*
 * Copyright (C) 2009 Ronny Yabar Aizcorbe <ronnycontacto@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>



#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include "rmlib.h"

using namespace std;




int main() {

    char* ip = "127.0.0.1";

    rmlib* rmlib1;
    rmlib1 = new rmlib();

    rmlib1->rm_init(ip,1500,ip,8888);
    rmlib1->enviarDato("Dato dato");

    



}











