%% Drainage Channel
% ECE 523 hw9 Optimization
% Author:Qihao He Due date: 11/18/2016
% constrained optimization
% fmisearch 
clear;
a=0.05;
b=0.2;
c=1-2*a-2*b;
theta=pi/6;
phi=pi/6;
FS_xc=zeros(4,1);
truesol=[0.2;0.2;pi/5;2*pi/5];
f=@(x) (1-2*x(1)-2*x(2))*(x(1)*sin(x(4))+x(2)*sin(x(3)))/2+...
    x(1)*sin(x(4))*x(2)*cos(x(3))+x(2)^2*sin(x(3))*cos(x(3))/2+...
    x(1)^2*sin(x(4))*cos(x(4))/2;
    % area
g=@(x) -f(x);
xo=[a;b;theta;phi];
% options = optimset('Display','iter','TolX',1e-6,'TolFun',1e-6,'MaxFunEvals',1e4);
options = optimset('TolX',1e-6,'TolFun',1e-6,'MaxFunEvals',1e4);
[FS_xc,FS_fxc,FS_exitflag,FS_output]=fminsearch(g,xo,options);
[SD_xc,SD_fxc,SD_exitflag,SD_iter,SD_funeval]=DrainageChannel_SD(g,xo);
FS_error=norm(FS_xc-truesol)/eps
SD_error=norm(SD_xc-truesol)/eps
SD_iter
SD_funeval
%% Results

% fminsearch:
%  Iteration   Func-count     min f(x)         Procedure
% 
%    220          377       -0.0769421         contract outside
% Steepest Descent:
%   Iteration   Func-count     min f(x)  
%    328        2657        -0.0769421

% Which requires the most iterations?
% The Steepest Descent method requires the most iterations. Its iteration
% time is 328. Fminsearch method needs 220 iteration.

% Which is more accurate for the same error criteria?
% The fminsearch is more accurate for the same error criteria, its relative
% error compare with Epsilon is 2.5743e+09 while the Steepest descent
% error is 2.5600e+11.

% What you learned from doing this work?
% I learned about the usage of fminsearch for searching the maximum value.
% Also I used the numerical method to calculate the gradient of the
% function, which is using the step in the vector x to calculate the
% partial dirivative of the function and that makes the gradient.

% Can you get any algorithm to converge if you let all initial conditions
% be equal to zero?

% Fminsearch and Steepest descent both did not converge with initial
% conditions equal to zero.