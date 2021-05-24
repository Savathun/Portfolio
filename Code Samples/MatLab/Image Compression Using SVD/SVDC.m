clear
clc
a=imread('a.jpg');
[m,n,d]=size(a);
kmax=floor((m*n)/(m+n+1));
da=double(a);
U=zeros(m,m);S=zeros(m,n);V=zeros(n,n);e=zeros(kmax,d);cr=zeros(kmax,1);rmse=zeros(kmax,d);
for i=1:d
    [U(:,:,i),S(:,:,i),V(:,:,i)]=svd(da(:,:,i));
end
for k=1:kmax
    ca=zeros(m,n,d);
    cr(k)=m*n/(k*(m+n+1));
    for i=1:d
        cai=zeros(m,n,d);
        [ca(:,:,i),cai(:,:,i)]=deal(U(:,1:k,i)*S(1:k,1:k,i)*V(:,1:k,i)');
        e(k,i)=S(k+1,k+1,i)/S(1,1,i);
        rmse(k,i)=sqrt(sum(sum(((da(:,:,i)-ca(:,:,i)).^2)))/(m*n));
        imwrite(uint8(cai), sprintf('%dk%d.jpg',k,i));
    end
    imwrite(uint8(ca), sprintf('%dk.jpg', k));
end
figure
p=plot(1:kmax,e,'MarkerEdgeColor','r','MarkerEdgeColor','g');
set(p,{'color'},{'red';'green';'blue'})
xlabel('Approximation Rank k');
ylabel('Relative 2-Norm');
xlim([1 kmax])
legend('Red','Green','Blue')
grid on

figure
p=plot(1:kmax,rmse,'MarkerEdgeColor','r','MarkerEdgeColor','g');
set(p,{'color'},{'red';'green';'blue'})
xlabel('Approximation Rank k');
ylabel('RMS Erorr');
xlim([1 kmax])
legend('Red','Green','Blue')
grid on

figure
plot(1:kmax,cr);
xlabel('Approximation Rank k');
ylabel('Compression Ratio');
xlim([1 kmax])
grid on