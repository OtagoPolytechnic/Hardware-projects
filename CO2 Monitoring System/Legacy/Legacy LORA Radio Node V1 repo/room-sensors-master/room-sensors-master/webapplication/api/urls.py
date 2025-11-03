from django.urls import path
from api import views

urlpatterns = [
    path('sensors/', views.sensor_list),
    path('sensors/<int:pk>/', views.sensor_details),
    path('history/', views.history_list),
    path('users/', views.UserList.as_view()),
    path('users/<int:pk>', views.UserDetail.as_view()),
]