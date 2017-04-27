%% Trapezoidal Rule
% ECE 523 hw10 Ordinary Differential Equations Homework
% Author:Qihao He Due date: 11/28/2016
% [xc,fxc,exitflag,iter,funeval]=TR(fun1,fun2,M(matrix),xo(initial),h(stepsize),t0(start),t1(end))
function [tspan,xc,exitflag]=TR(M,xo,h,t0,t1)
%-----~ Initialize Variables ~----
TOL=1e6*eps;
xk=xo;% store the initial guess xo
tspan=t0:h:t1;%initialize the time span
m=size(tspan,2);%get the time span size
n=size(xk,1);% get the size of xk
exitflag=0;
xkp1=zeros(n,1);% xk+1
xc=zeros(n,m);% initilize the output xc
dxk=zeros(n,1);
x=zeros(n,1);
tk=t0;
%-----~ Iterative Body ~------
for k=1:m% prevent infinit loop
    warning('off','all')
%-----~ store vector ~-----
    xc(:,k)=xk;
    dxk=M*xk;
%-----~ Backward Euler ~-----
    if k==1% use the BE only for the first time to break into inplicit
        xkp1=(1-h*M)\xk;%x=A\y,g=@(x) x-xk-h*M*x
    end
%-----~ optimize using Newton method ~-----
    for n=1:1e2
        J=(M*xkp1-M*xk)/h;
        H=J\(M*xk);
        xkp1=xk-H;
        if norm(xk-xkp1)<=TOL
            xkp1=xk;
            break;
        end
    end
    dxkp1=M*xkp1;
    xkp1=xk+(h/2)*(dxk+dxkp1);
    
    %-----~ Increment ~-----
    xk=xkp1;
    tk=tk+h;
end
exitflag=1;
warning('on','all');