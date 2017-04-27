%% Forward Euler Algorithm
% ECE 523 hw10 Ordinary Differential Equations Homework
% Author:Qihao He Due date: 11/28/2016
% [xc,fxc,exitflag,iter,funeval]=FEA(fun1,fun2,M(matrix),xo(initial),h(stepsize),t0(start),t1(end))
function [tspan,xc,exitflag]=FE(M,xo,h,t0,t1)
%-----~ Initialize Variables ~----
xk=xo;% store the initial guess xo
tspan=t0:h:t1;%initialize the time span
m=size(tspan,2);%get the time span size
n=size(xk,1);% get the size of xk
exitflag=0;
xkp1=zeros(n,1);% xk+1
xc=zeros(n,m);% initilize the output xc
dxk=zeros(n,1);
tk=t0;
%-----~ Iterative Body ~------
for k=1:m% prevent infinit loop
    %-----~ store vector ~-----
    xc(:,k)=xk;
    dxk=M*xk;
    xkp1=xk+h*dxk;
    %-----~ Increment ~-----
    xk=xkp1;
    tk=tk+h;
end
exitflag=1;