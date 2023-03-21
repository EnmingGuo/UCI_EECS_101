% image1.raw: {u=49.422600, var=15.144367}
% image2.raw: {u=79.478500, var=21.492956}
% image3.raw: {u=110.721100, var=26.886707}
% image4.raw: {u=160.079193, var=35.986778}

u=[49.422600,79.478500,110.721100,160.079193];
var=[15.144367,21.492956,26.886707,35.986778];
coefficients = polyfit(u, var, 1);
yFitted = polyval(coefficients, u);
plot(u,yFitted,'b',u,var,'ro');
xlabel('u');
ylabel('var');
grid on;
fprintf('A: %.5f C^2: %.5f',coefficients(1),coefficients(2));

fprintf('')
legend("Fit Result","Original Data")