function showboxes_on_im( boxes, clr_in)

% showboxes(im, boxes, out)
% Draw bounding boxes on top of image.
% If out is given, a pdf of the image is generated (requires export_fig).
print = false;
cwidth = 6;

%image(im);

%axis image;
%axis off;
%set(gcf, 'Color', 'white');

if ~isempty(boxes)
    numfilters = floor(size(boxes, 2)/4);
    if print
        % if printing, increase the contrast around the boxes
        % by printing a white box under each color box
        for i = 1:numfilters
            x1 = boxes(:,1+(i-1)*4);
            y1 = boxes(:,2+(i-1)*4);
            x2 = boxes(:,3+(i-1)*4);
            y2 = boxes(:,4+(i-1)*4);
            % remove unused filters
            del = find(((x1 == 0) .* (x2 == 0) .* (y1 == 0) .* (y2 == 0)) == 1);
            x1(del) = [];
            x2(del) = [];
            y1(del) = [];
            y2(del) = [];
            if i == 1
                w = wwidth;
            else
                w = wwidth;
            end
            line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', 'w', 'linewidth', w);
        end
    end
    % draw the boxes with the detection window on top (reverse order)
    for i = numfilters:-1:1
        x1 = boxes(:,1+(i-1)*4);
        y1 = boxes(:,2+(i-1)*4);
        x2 = boxes(:,3+(i-1)*4);
        y2 = boxes(:,4+(i-1)*4);
        % remove unused filters
        del = find(((x1 == 0) .* (x2 == 0) .* (y1 == 0) .* (y2 == 0)) == 1);
        x1(del) = [];
        x2(del) = [];
        y1(del) = [];
        y2(del) = [];
        if i == 1
            c = [160/255 0 0];
            s = '-';
        else
            c = 'b';
            s = '-';
        end
        line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', clr_in, 'linewidth', cwidth, 'linestyle', s);
    end
end

