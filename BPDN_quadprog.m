function [x] = BPDN_quadprog(y, A, tao)
    [y_rows,y_columns] = size(y);    
    if y_rows<y_columns
        y = y';%y should be a column vector    
    end   
    n = size(A,2);  
    %according to section II-A of the reference  
    b = A'*y;  
    c = tao*ones(2*n,1)+[-b;b];  
    B = [A'*A,-A'*A;-A'*A,A'*A];  
    lb = zeros(2*n,1);  
    z0 = quadprog(B,c,[],[],[],[],lb);  
    x = z0(1:n) - z0(n+1:2*n);

end

