clear all;
close all;

data=load('path.txt');

% Data reduction
interval = 1000;
x = data(1:interval:end,1);
y = data(1:interval:end,2);
theta = data(1:interval:end,3);

%Arrow Parameters
pointsize = 100; %pointsize of initial and goal points
ArrowLength = 10;
LineWidth = 1; %LineWidth of the arrow
MaxHeadSize = 2;
u = ArrowLength .* cos(theta); % convert polar (theta,r) to cartesian
v = ArrowLength .* sin(theta);

%Boundary Parameters
length = size(x,1); %Length of the sample
color_start = [252, 70, 107]/255;
color_end = [63, 94, 251]/255;
%color_obstacle = [228, 230, 43]/255;
color_obstacle = [200, 200, 200]/255;
colors_g = [linspace(color_start(1),color_end(1),length)', linspace(color_start(2),color_end(2),length)', linspace(color_start(3),color_end(3),length)'];
xsize1 = 20;
xsize2 = 640;
ysize1 = -440;
ysize2 = -20;


figure;
xlim([xsize1-50 xsize2+50]);
ylim([ysize1-50 ysize2+50]);

hold on;
%Boundary
rectangle('Position',[xsize1-10 ysize1-10 xsize2+20 -ysize1+20], 'FaceColor',color_obstacle);
rectangle('Position',[xsize1 ysize1 xsize2 -ysize1],'FaceColor',[1 1 1]);

% Big Obstacle
% rectangle('Position',[310 -290 350 290], 'FaceColor',color_obstacle);

%Obstacles
rectangle('Position',[100 -200 100 100], 'FaceColor',color_obstacle);
rectangle('Position',[300 -300 100 100], 'FaceColor',color_obstacle);
draw_circle(150,-350,50,color_obstacle);
draw_circle(550,-150,50,color_obstacle);
draw_circle(500,-300,50,color_obstacle);

%Start/Goal Points
scatter(x(1),y(1),pointsize,color_start,'filled');
scatter(x(end),y(end),pointsize,color_end,'filled');
text(x(1)-25, y(1)-20, sprintf('Start: (%d, %d)',round(x(1)),round(y(1))),'Color',color_start,'FontSize',13,'LineWidth',1,'EdgeColor','k', 'BackgroundColor','w');
text(x(end)-25, y(end)-15, sprintf('End: (%d, %d)',round(x(end)),round(y(end))),'Color',color_end,'FontSize',13,'LineWidth',1,'EdgeColor','k', 'BackgroundColor','w');

pause(20);
for i=1:length
    quiver(x(i),y(i),u(i),v(i),'color', colors_g(i,:),'LineWidth',LineWidth,'MaxHeadSize',MaxHeadSize);
    %scatter(x(i),y(i),50,colors_g(i,:),'filled')
    %uistack(t1,'top');
    %uistack(t2,'top');
    text(x(1)-25, y(1)-20, sprintf('Start: (%d, %d)',round(x(1)),round(y(1))),'Color',color_start,'FontSize',13,'LineWidth',1,'EdgeColor','k', 'BackgroundColor','w');
    text(x(end)-25, y(end)-15, sprintf('Start: (%d, %d)',round(x(end)),round(y(end))),'Color',color_end,'FontSize',13,'LineWidth',1,'EdgeColor','k', 'BackgroundColor','w');
    pause(0.02);
end
hold off;

% figure;
% plot3(data(:,1), data(:,2), data(:,3), 'o-');
% axis equal, grid on, rotate3d on;
% 
% figure;
% plot(data(:,1), data(:,2), 'o-');
% axis equal, grid on, rotate3d on;

function draw_circle(x,y,r,color)
%viscircles([150 -350],50,'Color',color_obstacle);
c = [x y];
pos = [c-r 2*r 2*r];
rectangle('Position',pos,'Curvature',[1 1],'FaceColor',color);
axis equal
end