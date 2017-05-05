function [box_x_1, box_x_2, box_y_1, box_y_2] = getBox(total_x, total_y, scale)


scale_x = total_x * scale;
scale_y = total_y * scale;

box_x_1 = (total_x - scale_x) / 2;
box_x_2 = box_x_1 + scale_x;

box_y_1 = (total_y - scale_y) / 2;
box_y_2 = box_y_1 + scale_y;
