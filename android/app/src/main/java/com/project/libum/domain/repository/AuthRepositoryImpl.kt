package com.project.libum.domain.repository

import android.util.Log
import com.project.libum.core.exeption.login.IncorrectPasswordException
import com.project.libum.core.exeption.login.NetworkErrorException
import com.project.libum.data.local.dao.UserEntity
import com.project.libum.data.model.LoginRequest
import com.project.libum.data.remote.ApiClient.apiService


class AuthRepositoryImpl(
    private val userCacheRepositoryImpl: UserCacheRepository
) : AuthRepository {

    override suspend fun login(
        request: LoginRequest
    ): Result<UserEntity> {
        try {
            val response = apiService.login(request)

            Log.d("AuthRepositoryImpl", "loginCached: response code: ${response.code()}")
            if (response.isSuccessful) {
                return response.body()?.let { loginResponse ->

                    val user = UserEntity(
                        id = loginResponse.id,
                        email = loginResponse.email,
                        password = request.password,
                        login = loginResponse.login
                    )

                    Result.success(user)
                } ?: Result.failure(Exception("Empty response"))
            } else {
                return when(response.code()){
                    401 -> Result.failure(IncorrectPasswordException("Incorrect password"))
                    404 -> Result.failure(NetworkErrorException("Network error"))
                    else -> Result.failure(Exception("Login failed"))
                }
            }
        } catch (e: Exception) {
            return  Result.failure(e)
        }
    }

    override suspend fun loginCached(): Result<UserEntity> {

        val user = userCacheRepositoryImpl.getUserData()
        val res = login(LoginRequest(user.email, user.password))

        res.onFailure {
            throw res.exceptionOrNull()!!
        }

        return res
    }

    override suspend fun logout() {
        userCacheRepositoryImpl.clearUserLoinData()
    }
}