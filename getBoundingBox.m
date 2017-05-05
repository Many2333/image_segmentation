function [box_x_min, box_x_max, box_y_min, box_y_max] = getBoundingBox(img_name)
    addpath('./xml_io_tools/');
    xml_path = ['./VOCdevkit/VOC2012/2012Annotations/', img_name, '.xml'];
    xml_file = xml_read(xml_path);
    
    object = xml_file.object;
    bndbox = object.bndbox;
    
    box_x_min = bndbox.ymin;
    box_x_max = bndbox.ymax;
    box_y_min = bndbox.xmin;
    box_y_max = bndbox.xmax;
end

