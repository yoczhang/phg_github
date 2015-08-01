/* Parallel Hierarchical Grid -- an adaptive finite element library.
 *
 * Copyright (C) 2005-2010 State Key Laboratory of Scientific and
 * Engineering Computing, Chinese Academy of Sciences. */

/* This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA */

//写了一个给向量类型 VEC *myvector 赋值，最终是可以赋值成功了，并在最后的打印部分测试了申请的向量的大小
#include "phg.h"

#if (PHG_VERSION_MAJOR <= 0 && PHG_VERSION_MINOR < 9)
# undef ELEMENT
typedef SIMPLEX ELEMENT;
typedef USE_MPI 1;
#endif
 
#include <string.h>
#include <math.h>


int
main(int argc, char *argv[])
{
    INT periodicity = 0 /* X_MASK | Y_MASK | Z_MASK */;

    char *fn = "../../test/cube4.dat";
    GRID *g;

    //MPI_Init(&argc,&argv);
    //MPI_Comm comm;
    //phgInitSetComm(comm);
    phgInit(&argc, &argv);

    g = phgNewGrid(-1);
    phgSetPeriodicity(g, periodicity);

    if (!phgImport(g, fn, TRUE))
	phgError(1, "can't read file \"%s\".\n", fn);

    /*-----------------------------------------------------------------*/
    /*--------------- the following is my test ------------------------*/
    int i=0;
    DOF *u,*a;
    static DOF_TYPE type;//=DOF_P0;
    type.np_vert=0;
    type.np_edge=0;
    type.np_face=1;
    type.np_elem=0;
    u=phgDofNew(g,DOF_P0,1,"u",DofNoAction);
    ELEMENT *e;
    MAP *mymap;
    printf("test-1----------\n");
    a=phgDofNew(u->g,&type,1,"a",DofNoAction);
    mymap=phgMapCreate(a,NULL);
    VEC *myvector;
    printf("test0------------\n");
    myvector=phgMapCreateVec(mymap,1);
    printf("test1-----------\n");
    phgVecDisassemble(myvector);
    /*
    ForAllElements(g,e){
        for(i=0;i<NFace;i++){
            phgVecAddEntry(myvector,0,e->faces[i],0.1);
        }
    }
    */
    printf("test1.1-----------\n");
    phgVecAddEntry(myvector,0,0,0.1);
    printf("test2----------\n");
    phgVecAddEntry(myvector,0,1,0.2);
    phgVecAssemble(myvector);
    printf("test3--------\n");
    phgMapVecToDofArrays(mymap,myvector,FALSE,&a,NULL);
    printf("test4-----------\n");
    
    FLOAT *v;
    v=myvector->data;
    printf("%f \n",v[0]);
    printf("%f \n",v[1]);
    printf("%f \n",v[2397]);//貌似申请的myvector向量的大小为2398，测试v[2398]时就报错了


    phgVecDestroy(&myvector);
    phgMapDestroy(&mymap);
    phgDofFree(&a);
    phgDofFree(&u);



    /*--------------- end of my test ----------------------------------*/
    /*-----------------------------------------------------------------*/
    
    phgFreeGrid(&g);

    phgFinalize();

    return 0;
}
