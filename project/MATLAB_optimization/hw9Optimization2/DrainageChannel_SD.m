%% Drainage Channel_SD
% ECE 523 hw9 Optimization
% Author:Qihao He Due date: 11/18/2016
% constrained optimization
function [xc,fxc,exitflag,iter,funeval]=DrainageChannel_SD(fun,xo)
%-----~ Initialize Variables ~----
xk=xo;% store the initial guess xo
n=size(xk,1);% get the size of xk
xkp1=zeros(n,1); % xk+1
xkh=zeros(n,1);% xk hat
exitflag=0;
xc=zeros(n,1);% initilize the output xc
fxc=0;% output feval xc
h=1e-6;% Gradient step
TOLX=1e-6; % Tolerance for x
TOLFUN=1e-6; % Tolerance for function
steptol=eps^(2/3)/2;% Tolerance for step
iter=0;% iteration times
funeval=0;% function evaluation times
%-----~ Iterative Body ~------
for k=1:1e6% prevent infinit loop
%-----~ Gradient ~------
    s=zeros(n,1);% reset gradient
    for j=1:n
        xkh=xk;% set xk hat
        xkh(j)=xk(j)+h;% set xkhat by j row add a step
        dfun=(fun(xkh)-fun(xk))/h;% partial dirivative
        funeval=funeval+2;
        s(j)=s(j)+dfun;% store gradient j row
    end
    s=-s;% take the negative gradient
    alpha=1;%line search set to 1
%-----~ Imperfect line search method ~----
    while fun(xk+alpha*s)>=fun(xk)% when the next guess greater than current guess
        funeval=funeval+2;
        alpha=alpha*0.5;% the next line search
        xkp1=xk+alpha*s;% next guess
    end
    xkp1=xk+alpha*s;
%-----~ Check Termination ~----
    if abs(fun(xkp1)-fun(xk))<max(TOLFUN*max(abs(fun(xkp1)),abs(fun(xk))),steptol)&&...
            max(abs(s))<TOLX*max(max(abs(xkp1),abs(xk)))% tolerance check
        funeval=funeval+4;
        exitflag=1;
        xc=xkp1;% output xkp1
        fxc=fun(xc);% output function evaluation xc
        funeval=funeval+1;
        break;
    end
%-----~ Increment ~-----
    xk=xkp1;% set xk
    iter=iter+1;% increment iteration
end